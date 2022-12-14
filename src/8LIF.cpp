#include "plugin.hpp"
#include "NeuronConstants.hpp"
#include <algorithm>
#include <deque>

struct _8LIF : Module {
	enum ParamId {
		W11_PARAM,
		W12_PARAM,
		W13_PARAM,
		W14_PARAM,
		W15_PARAM,
		W16_PARAM,
		W17_PARAM,
		W18_PARAM,
		W21_PARAM,
		W22_PARAM,
		W23_PARAM,
		W24_PARAM,
		W25_PARAM,
		W26_PARAM,
		W27_PARAM,
		W28_PARAM,
		W31_PARAM,
		W32_PARAM,
		W33_PARAM,
		W34_PARAM,
		W35_PARAM,
		W36_PARAM,
		W37_PARAM,
		W38_PARAM,
		W41_PARAM,
		W42_PARAM,
		W43_PARAM,
		W44_PARAM,
		W45_PARAM,
		W46_PARAM,
		W47_PARAM,
		W48_PARAM,
		W51_PARAM,
		W52_PARAM,
		W53_PARAM,
		W54_PARAM,
		W55_PARAM,
		W56_PARAM,
		W57_PARAM,
		W58_PARAM,
		W61_PARAM,
		W62_PARAM,
		W63_PARAM,
		W64_PARAM,
		W65_PARAM,
		W66_PARAM,
		W67_PARAM,
		W68_PARAM,
		W71_PARAM,
		W72_PARAM,
		W73_PARAM,
		W74_PARAM,
		W75_PARAM,
		W76_PARAM,
		W77_PARAM,
		W78_PARAM,
		W81_PARAM,
		W82_PARAM,
		W83_PARAM,
		W84_PARAM,
		W85_PARAM,
		W86_PARAM,
		W87_PARAM,
		W88_PARAM,
		LEAK_PARAM,
		REF_PARAM,
		DELAY_PARAM,
		FIRE_PARAM,
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


	float mem[OUTPUTS_LEN] = {0.f};
	bool risen[INPUTS_LEN] = {false};
	bool old_pressed = false;
	int ref_timer[OUTPUTS_LEN] = {0};
	std::deque<int> deqlay[OUTPUTS_LEN];
	dsp::PulseGenerator fire[OUTPUTS_LEN];

	_8LIF() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(W11_PARAM, -w_max, w_max, 0.f, "");
		configParam(W12_PARAM, -w_max, w_max, 0.f, "");
		configParam(W13_PARAM, -w_max, w_max, 0.f, "");
		configParam(W14_PARAM, -w_max, w_max, 0.f, "");
		configParam(W15_PARAM, -w_max, w_max, 0.f, "");
		configParam(W16_PARAM, -w_max, w_max, 0.f, "");
		configParam(W17_PARAM, -w_max, w_max, 0.f, "");
		configParam(W18_PARAM, -w_max, w_max, 0.f, "");
		configParam(W21_PARAM, -w_max, w_max, 0.f, "");
		configParam(W22_PARAM, -w_max, w_max, 0.f, "");
		configParam(W23_PARAM, -w_max, w_max, 0.f, "");
		configParam(W24_PARAM, -w_max, w_max, 0.f, "");
		configParam(W25_PARAM, -w_max, w_max, 0.f, "");
		configParam(W26_PARAM, -w_max, w_max, 0.f, "");
		configParam(W27_PARAM, -w_max, w_max, 0.f, "");
		configParam(W28_PARAM, -w_max, w_max, 0.f, "");
		configParam(W31_PARAM, -w_max, w_max, 0.f, "");
		configParam(W32_PARAM, -w_max, w_max, 0.f, "");
		configParam(W33_PARAM, -w_max, w_max, 0.f, "");
		configParam(W34_PARAM, -w_max, w_max, 0.f, "");
		configParam(W35_PARAM, -w_max, w_max, 0.f, "");
		configParam(W36_PARAM, -w_max, w_max, 0.f, "");
		configParam(W37_PARAM, -w_max, w_max, 0.f, "");
		configParam(W38_PARAM, -w_max, w_max, 0.f, "");
		configParam(W41_PARAM, -w_max, w_max, 0.f, "");
		configParam(W42_PARAM, -w_max, w_max, 0.f, "");
		configParam(W43_PARAM, -w_max, w_max, 0.f, "");
		configParam(W44_PARAM, -w_max, w_max, 0.f, "");
		configParam(W45_PARAM, -w_max, w_max, 0.f, "");
		configParam(W46_PARAM, -w_max, w_max, 0.f, "");
		configParam(W47_PARAM, -w_max, w_max, 0.f, "");
		configParam(W48_PARAM, -w_max, w_max, 0.f, "");
		configParam(W51_PARAM, -w_max, w_max, 0.f, "");
		configParam(W52_PARAM, -w_max, w_max, 0.f, "");
		configParam(W53_PARAM, -w_max, w_max, 0.f, "");
		configParam(W54_PARAM, -w_max, w_max, 0.f, "");
		configParam(W55_PARAM, -w_max, w_max, 0.f, "");
		configParam(W56_PARAM, -w_max, w_max, 0.f, "");
		configParam(W57_PARAM, -w_max, w_max, 0.f, "");
		configParam(W58_PARAM, -w_max, w_max, 0.f, "");
		configParam(W61_PARAM, -w_max, w_max, 0.f, "");
		configParam(W62_PARAM, -w_max, w_max, 0.f, "");
		configParam(W63_PARAM, -w_max, w_max, 0.f, "");
		configParam(W64_PARAM, -w_max, w_max, 0.f, "");
		configParam(W65_PARAM, -w_max, w_max, 0.f, "");
		configParam(W66_PARAM, -w_max, w_max, 0.f, "");
		configParam(W67_PARAM, -w_max, w_max, 0.f, "");
		configParam(W68_PARAM, -w_max, w_max, 0.f, "");
		configParam(W71_PARAM, -w_max, w_max, 0.f, "");
		configParam(W72_PARAM, -w_max, w_max, 0.f, "");
		configParam(W73_PARAM, -w_max, w_max, 0.f, "");
		configParam(W74_PARAM, -w_max, w_max, 0.f, "");
		configParam(W75_PARAM, -w_max, w_max, 0.f, "");
		configParam(W76_PARAM, -w_max, w_max, 0.f, "");
		configParam(W77_PARAM, -w_max, w_max, 0.f, "");
		configParam(W78_PARAM, -w_max, w_max, 0.f, "");
		configParam(W81_PARAM, -w_max, w_max, 0.f, "");
		configParam(W82_PARAM, -w_max, w_max, 0.f, "");
		configParam(W83_PARAM, -w_max, w_max, 0.f, "");
		configParam(W84_PARAM, -w_max, w_max, 0.f, "");
		configParam(W85_PARAM, -w_max, w_max, 0.f, "");
		configParam(W86_PARAM, -w_max, w_max, 0.f, "");
		configParam(W87_PARAM, -w_max, w_max, 0.f, "");
		configParam(W88_PARAM, -w_max, w_max, 0.f, "");
		configParam(LEAK_PARAM, 0.f, l_max, 0.f, "Leakage");
		configParam(REF_PARAM, 0.f, r_max, 0.f, "Refractory period");
		configParam(DELAY_PARAM, 0.f, d_max, 0.f, "Delay");
		configParam(FIRE_PARAM, false, true, false, "FIRE");
		configInput(IN1_INPUT, "IN 1");
		configInput(IN2_INPUT, "IN 2");
		configInput(IN3_INPUT, "IN 3");
		configInput(IN4_INPUT, "IN 4");
		configInput(IN5_INPUT, "IN 5");
		configInput(IN6_INPUT, "IN 6");
		configInput(IN7_INPUT, "IN 7");
		configInput(IN8_INPUT, "IN 8");
		configOutput(OUT1_OUTPUT, "Out 1");
		configOutput(OUT2_OUTPUT, "Out 2");
		configOutput(OUT3_OUTPUT, "Out 3");
		configOutput(OUT4_OUTPUT, "Out 4");
		configOutput(OUT5_OUTPUT, "Out 5");
		configOutput(OUT6_OUTPUT, "Out 6");
		configOutput(OUT7_OUTPUT, "Out 7");
		configOutput(OUT8_OUTPUT, "Out 8");
	}

	void process(const ProcessArgs& args) override {
		bool spiking[INPUTS_LEN] = {false};
		for (int i = 0; i < INPUTS_LEN; i++)
		{
			float in_v = inputs[i].isConnected() ?
						 inputs[i].getVoltage()  :
						 outputs[i].getVoltage();
			
			if (in_v > V_THR)
			{
				if (!risen[i]) {
					risen[i] = true;
					spiking[i] = true;
				}
			}
			else {
				risen[i] = false;
			}
		}

		bool pressed = params[FIRE_PARAM].getValue() && !old_pressed;
		old_pressed = params[FIRE_PARAM].getValue();
		if (pressed)
		{
			for (int i = 0; i < OUTPUTS_LEN; i++)
			{
				mem[i] = V_RST;
				ref_timer[i] = params[REF_PARAM].getValue() * args.sampleRate;
				deqlay[i].push_back(params[DELAY_PARAM].getValue() * args.sampleRate);
			}
		}


		for (int h = 0; h < OUTPUTS_LEN; h++)
		{
			for (unsigned i=0; i<deqlay[h].size(); i++)
			{
				deqlay[h][i]--;
				if (deqlay[h][i] <= 0)
				{
					deqlay[h].erase(deqlay[h].begin() + i);
					fire[h].trigger(1e-3);
				}
			}
			outputs[h].setVoltage(fire[h].process(args.sampleTime) ? SPIKE_MAG : V_RST);

			if (ref_timer[h] > 0)
			{
				ref_timer[h]--;
			}
			else
			{
				for (int i = 0; i < INPUTS_LEN; i++)
				{
					if (spiking[i])
					{
						mem[h] += params[h + i*8].getValue();
					}
				}
				mem[h] -= params[LEAK_PARAM].getValue() * args.sampleTime;
				mem[h] = mem[h] < V_RST ? V_RST: mem[h];

				if (mem[h] > V_THR) // SPIKE!
				{
					mem[h] = V_RST;
					ref_timer[h] = params[REF_PARAM].getValue() * args.sampleRate;
					deqlay[h].push_back(params[DELAY_PARAM].getValue() * args.sampleRate);
				}
			}
		}
	}
};


struct _8LIFWidget : ModuleWidget {
	_8LIFWidget(_8LIF* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/8LIF.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 28.5)), module, _8LIF::W11_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 28.5)), module, _8LIF::W12_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 28.5)), module, _8LIF::W13_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 28.5)), module, _8LIF::W14_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 28.5)), module, _8LIF::W15_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 28.5)), module, _8LIF::W16_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 28.5)), module, _8LIF::W17_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 28.5)), module, _8LIF::W18_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 38.5)), module, _8LIF::W21_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 38.5)), module, _8LIF::W22_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 38.5)), module, _8LIF::W23_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 38.5)), module, _8LIF::W24_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 38.5)), module, _8LIF::W25_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 38.5)), module, _8LIF::W26_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 38.5)), module, _8LIF::W27_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 38.5)), module, _8LIF::W28_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 48.5)), module, _8LIF::W31_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 48.5)), module, _8LIF::W32_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 48.5)), module, _8LIF::W33_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 48.5)), module, _8LIF::W34_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 48.5)), module, _8LIF::W35_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 48.5)), module, _8LIF::W36_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 48.5)), module, _8LIF::W37_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 48.5)), module, _8LIF::W38_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 58.5)), module, _8LIF::W41_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 58.5)), module, _8LIF::W42_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 58.5)), module, _8LIF::W43_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 58.5)), module, _8LIF::W44_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 58.5)), module, _8LIF::W45_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 58.5)), module, _8LIF::W46_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 58.5)), module, _8LIF::W47_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 58.5)), module, _8LIF::W48_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 68.5)), module, _8LIF::W51_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 68.5)), module, _8LIF::W52_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 68.5)), module, _8LIF::W53_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 68.5)), module, _8LIF::W54_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 68.5)), module, _8LIF::W55_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 68.5)), module, _8LIF::W56_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 68.5)), module, _8LIF::W57_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 68.5)), module, _8LIF::W58_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 78.5)), module, _8LIF::W61_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 78.5)), module, _8LIF::W62_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 78.5)), module, _8LIF::W63_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 78.5)), module, _8LIF::W64_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 78.5)), module, _8LIF::W65_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 78.5)), module, _8LIF::W66_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 78.5)), module, _8LIF::W67_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 78.5)), module, _8LIF::W68_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 88.5)), module, _8LIF::W71_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 88.5)), module, _8LIF::W72_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 88.5)), module, _8LIF::W73_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 88.5)), module, _8LIF::W74_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 88.5)), module, _8LIF::W75_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 88.5)), module, _8LIF::W76_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 88.5)), module, _8LIF::W77_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 88.5)), module, _8LIF::W78_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 98.5)), module, _8LIF::W81_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(30.5, 98.5)), module, _8LIF::W82_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 98.5)), module, _8LIF::W83_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 98.5)), module, _8LIF::W84_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 98.5)), module, _8LIF::W85_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(70.5, 98.5)), module, _8LIF::W86_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 98.5)), module, _8LIF::W87_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(90.5, 98.5)), module, _8LIF::W88_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(25.5, 118.0)), module, _8LIF::LEAK_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(45.5, 118.0)), module, _8LIF::REF_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(65.5, 118.0)), module, _8LIF::DELAY_PARAM));
		addParam(createParamCentered<BefacoPush>(mm2px(Vec(85.5, 118.0)), module, _8LIF::FIRE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 28.5)), module, _8LIF::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 38.5)), module, _8LIF::IN2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 48.5)), module, _8LIF::IN3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 58.5)), module, _8LIF::IN4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 68.5)), module, _8LIF::IN5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 78.5)), module, _8LIF::IN6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 88.5)), module, _8LIF::IN7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 98.5)), module, _8LIF::IN8_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.5, 108.5)), module, _8LIF::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 108.5)), module, _8LIF::OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(40.5, 108.5)), module, _8LIF::OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.5, 108.5)), module, _8LIF::OUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(60.5, 108.5)), module, _8LIF::OUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 108.5)), module, _8LIF::OUT6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(80.5, 108.5)), module, _8LIF::OUT7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 108.5)), module, _8LIF::OUT8_OUTPUT));
	}
};


Model* model_8LIF = createModel<_8LIF, _8LIFWidget>("8LIF");