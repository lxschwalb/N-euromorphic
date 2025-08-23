#include "plugin.hpp"
#include "NeuronConstants.hpp"
#include <algorithm>
#include <deque>

struct ReservoirPlus : Module {
	enum ParamId {
		W_VAR_PARAM,
		W_AVG_PARAM,
		NUM_NEURONS_PARAM,
		DIST_PARAM,
		L_VAR_PARAM,
		L_AVG_PARAM,
		R_VAR_PARAM,
		R_AVG_PARAM,
		APPLY_PARAM,
		D_VAR_PARAM,
		D_AVG_PARAM,
		NEURON_OFFSET_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		IN5_INPUT,
		IN6_INPUT,
		IN7_INPUT,
		IN8_INPUT,
		APPLY_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		OUT5_OUTPUT,
		OUT6_OUTPUT,
		OUT7_OUTPUT,
		OUT8_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};
	
	float mem[256] = {0.f};
	bool risen[8] = {false};
	int ref_timer[256] = {0};
	dsp::PulseGenerator fire[8];
	bool spiking[256][256] = {{false}};
	int num_neurons=8;

	static constexpr int MAX_DELAY_EVENTS = 64;
	int deqlay[256][256][MAX_DELAY_EVENTS] = {{0}}; // the ring buffer storage
	int deqlay_head[256][256] = {{0}}; // index to pop from
	int deqlay_tail[256][256] = {{0}}; // index to push to

	float weights[256][256] = {{0.f}};
	float leakages[256] = {0.f};
	float delays[256][256] = {{0.f}};
	float ref_periods[256] = {0.f};

	ReservoirPlus() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(W_VAR_PARAM, 0.f, 0.3f, 0.3f, ""); // weights
		configParam(W_AVG_PARAM, -0.3f, 0.3f, 0.f, "");
		configParam(NUM_NEURONS_PARAM, 8, 256, 8, "");
		configParam(NEURON_OFFSET_PARAM, 0, 248, 0, "");
		configParam(DIST_PARAM, 0.f, 1.f, 0.f, ""); // gaussian vs uniform
		configParam(L_VAR_PARAM, 0.f, 1.f, 0.f, ""); // leakage
		configParam(L_AVG_PARAM, 0.f, 0.15f, 0.15f, "");
		configParam(R_VAR_PARAM, 0.f, 1.f, 0.f, ""); // ref period
		configParam(R_AVG_PARAM, 0.f, 1.f, 1.f, "");
		configParam(APPLY_PARAM, 0.f, 1.f, 0.f, "");
		configParam(D_VAR_PARAM, 0.f, 1.f, 1.f, ""); // delay
		configParam(D_AVG_PARAM, 0.f, 1.f, 1.f, "");
		configInput(IN1_INPUT, "");
		configInput(IN2_INPUT, "");
		configInput(IN3_INPUT, "");
		configInput(IN4_INPUT, "");
		configInput(IN5_INPUT, "");
		configInput(IN6_INPUT, "");
		configInput(IN7_INPUT, "");
		configInput(IN8_INPUT, "");
		configInput(APPLY_INPUT, "");
		configOutput(OUT1_OUTPUT, "");
		configOutput(OUT2_OUTPUT, "");
		configOutput(OUT3_OUTPUT, "");
		configOutput(OUT4_OUTPUT, "");
		configOutput(OUT5_OUTPUT, "");
		configOutput(OUT6_OUTPUT, "");
		configOutput(OUT7_OUTPUT, "");
		configOutput(OUT8_OUTPUT, "");
	}

	float myrandom(float avg, float var, float dist, bool clamp_neg = false) {
		float uniform = avg + var * (random::uniform() * 2.f - 1.f);
		float gaussian = avg + var * random::normal();
		float result = dist * gaussian + (1.f - dist) * uniform;
		if (clamp_neg && result < 0.f) {
			result = -result;
		}
		return result;
	}

	void process(const ProcessArgs& args) override {
		if (params[APPLY_PARAM].getValue() || (inputs[APPLY_INPUT].isConnected() && inputs[APPLY_INPUT].getVoltage() > 0.5f))
		{
			num_neurons = params[NUM_NEURONS_PARAM].getValue();
			float dist  = params[DIST_PARAM].getValue();
			float w_var = params[W_VAR_PARAM].getValue();
			float w_avg = params[W_AVG_PARAM].getValue();
			float l_var = params[L_VAR_PARAM].getValue();
			float l_avg = params[L_AVG_PARAM].getValue();
			float r_var = params[R_VAR_PARAM].getValue();
			float r_avg = params[R_AVG_PARAM].getValue();
			float d_var = params[D_VAR_PARAM].getValue();
			float d_avg = params[D_AVG_PARAM].getValue();
			for (int i = 0; i < num_neurons; i++)
			{
				for (int j = 0; j < num_neurons; j++)
				{
					weights[i][j] = myrandom(w_avg, w_var, dist);
					delays[i][j]  = myrandom(d_avg, d_var, dist, true) * args.sampleRate;
				}
				leakages[i] 	= myrandom(l_avg, l_var, dist, true) * args.sampleTime;
				ref_periods[i] 	= myrandom(r_avg, r_var, dist, true) * args.sampleRate;
			}
		}

		int neuron_offset = params[NEURON_OFFSET_PARAM].getValue();

		for (int src = 0; src < num_neurons; src++) // process each neuron
		{
			for (int dst = 0; dst < num_neurons; dst++)
			{
				spiking[dst][src] = false;

				if (deqlay_head[src][dst] != deqlay_tail[src][dst]) { // buffer not empty
					// Decrement all elements in the buffer
					int count = (deqlay_tail[src][dst] - deqlay_head[src][dst] + MAX_DELAY_EVENTS) % MAX_DELAY_EVENTS;
					for (int j = 0; j < count; ++j) {
						int idx = (deqlay_head[src][dst] + j) % MAX_DELAY_EVENTS;
						deqlay[src][dst][idx]--;
					}
					// Check if the front element is ready
					if (deqlay[src][dst][deqlay_head[src][dst]] <= 0) {
						deqlay_head[src][dst] = (deqlay_head[src][dst] + 1) % MAX_DELAY_EVENTS; // pop front
						spiking[dst][src] = true;
					}
				}
			}
			if (ref_timer[src] <= 0)
			{			
				// integrate inputs
				mem[src] += std::inner_product(
					spiking[src], spiking[src] + num_neurons,
					weights[src],
					0.0f,
					std::plus<float>(),
					[](bool spike, float weight) { return spike ? weight : 0.0f; }
				);
				mem[src] -= leakages[src];
				mem[src] = mem[src] < V_RST ? V_RST: mem[src];

				if (mem[src] > V_THR) // SPIKE!
				{
					mem[src] = V_RST;
					ref_timer[src] = ref_periods[src];
					for (int dst = 0; dst < num_neurons; dst++)
					{
						deqlay[src][dst][deqlay_tail[src][dst]] = delays[src][dst];
						deqlay_tail[src][dst] = (deqlay_tail[src][dst] + 1) % MAX_DELAY_EVENTS;
					}					
					if (src >= neuron_offset && src < neuron_offset + 8)
					{
						fire[src-neuron_offset].trigger(1e-3);
					}
				}
			}
			else // wait out refractory period
			{
				ref_timer[src]--;
			}

			if(src >= neuron_offset && src < neuron_offset + 8)
			{
				float in_v = inputs[src-neuron_offset].isConnected() ?
							 inputs[src-neuron_offset].getVoltage()  :
							 0;
			
				if (in_v > V_THR) // Immediately fire on positive edge on input
				{
					if (!risen[src-neuron_offset]) {
						risen[src-neuron_offset] = true;
						fire[src-neuron_offset].trigger(1e-3);
						for (int dst = 0; dst < num_neurons; dst++)
						{
							deqlay[src][dst][deqlay_tail[src][dst]] = delays[src][dst];
							deqlay_tail[src][dst] = (deqlay_tail[src][dst] + 1) % MAX_DELAY_EVENTS;
						}
					}
				}
				else {
					risen[src-neuron_offset] = false;
				}
				outputs[src-neuron_offset].setVoltage(fire[src-neuron_offset].process(args.sampleTime) ? SPIKE_MAG : V_RST);
			}
		}		
	}
};


struct ReservoirPlusWidget : ModuleWidget {
	ReservoirPlusWidget(ReservoirPlus* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/ReservoirPlus.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 118.0)), module, ReservoirPlus::W_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 108.0)), module, ReservoirPlus::W_AVG_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(37.5, 33.5)), module, ReservoirPlus::NUM_NEURONS_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(63.5, 33.5)), module, ReservoirPlus::DIST_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 118.0)), module, ReservoirPlus::L_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 108.0)), module, ReservoirPlus::L_AVG_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 118.0)), module, ReservoirPlus::R_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 108.0)), module, ReservoirPlus::R_AVG_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 118.0)), module, ReservoirPlus::D_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 108.0)), module, ReservoirPlus::D_AVG_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(37.5, 46)), module, ReservoirPlus::NEURON_OFFSET_PARAM));
		addParam(createParamCentered<BefacoPush>(mm2px(Vec(50.5, 63.5)), module, ReservoirPlus::APPLY_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 28.5)), module, ReservoirPlus::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 38.5)), module, ReservoirPlus::IN2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 48.5)), module, ReservoirPlus::IN3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 58.5)), module, ReservoirPlus::IN4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 68.5)), module, ReservoirPlus::IN5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 78.5)), module, ReservoirPlus::IN6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 88.5)), module, ReservoirPlus::IN7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 98.5)), module, ReservoirPlus::IN8_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.5, 73.5)), module, ReservoirPlus::APPLY_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 28.5)), module, ReservoirPlus::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 38.5)), module, ReservoirPlus::OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 48.5)), module, ReservoirPlus::OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 58.5)), module, ReservoirPlus::OUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 68.5)), module, ReservoirPlus::OUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 78.5)), module, ReservoirPlus::OUT6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 88.5)), module, ReservoirPlus::OUT7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 98.5)), module, ReservoirPlus::OUT8_OUTPUT));
	}
};


Model* modelReservoirPlus = createModel<ReservoirPlus, ReservoirPlusWidget>("ReservoirPlus");