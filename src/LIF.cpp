#include "plugin.hpp"
#include "NeuronConstants.hpp"
#include <algorithm>
#include <deque>

struct LIF : Module {
	enum ParamId {
		W1_PARAM,
		W2_PARAM,
		W3_PARAM,
		W4_PARAM,
		W5_PARAM,
		W6_PARAM,
		W7_PARAM,
		W8_PARAM,
		W9_PARAM,
		W10_PARAM,
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
		IN9_INPUT,
		IN10_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		MEM_OUTPUT,
		FIRE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	float mem = 0.f;
	bool risen[INPUTS_LEN] = {false, false, false, false, false, false, false, false};
	bool old_pressed = false;
	int ref_timer = 0;
	std::deque<int> deqlay;
	dsp::PulseGenerator fire;

	LIF() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(W1_PARAM, -w_max, w_max, 0.f, "Weight 1");
		configParam(W2_PARAM, -w_max, w_max, 0.f, "Weight 2");
		configParam(W3_PARAM, -w_max, w_max, 0.f, "Weight 3");
		configParam(W4_PARAM, -w_max, w_max, 0.f, "Weight 4");
		configParam(W5_PARAM, -w_max, w_max, 0.f, "Weight 5");
		configParam(W6_PARAM, -w_max, w_max, 0.f, "Weight 6");
		configParam(W7_PARAM, -w_max, w_max, 0.f, "Weight 7");
		configParam(W8_PARAM, -w_max, w_max, 0.f, "Weight 8");
		configParam(W9_PARAM, -w_max, w_max, 0.f, "Weight 9");
		configParam(W10_PARAM, -w_max, w_max, 0.f, "Weight 10");
		configParam(LEAK_PARAM, 0.f, l_max, 0.f, "Leakage");
		configParam(REF_PARAM, 0.f, r_max, 0.f, "Refractory period");
		configParam(DELAY_PARAM, 0.f, d_max, 0.f, "Delay");
		configParam(FIRE_PARAM, false, true, false, "FIRE");
		configInput(IN1_INPUT, "In 1");
		configInput(IN2_INPUT, "In 2");
		configInput(IN3_INPUT, "In 3");
		configInput(IN4_INPUT, "In 4");
		configInput(IN5_INPUT, "In 5");
		configInput(IN6_INPUT, "In 6");
		configInput(IN7_INPUT, "In 7");
		configInput(IN8_INPUT, "In 8");
		configInput(IN9_INPUT, "In 9");
		configInput(IN10_INPUT, "In 10");
		configOutput(MEM_OUTPUT, "Membrane Potential");
		configOutput(FIRE_OUTPUT, "Fire");
	}

	void process(const ProcessArgs& args) override {
		for (unsigned i=0; i<deqlay.size(); i++)
		{
			deqlay[i]--;
			if (deqlay[i] <= 0)
			{
				deqlay.erase(deqlay.begin() + i);
				fire.trigger(1e-3);
			}
		}
		outputs[FIRE_OUTPUT].setVoltage(fire.process(args.sampleTime) ? SPIKE_MAG : V_RST);

		if (ref_timer > 0)
		{
			ref_timer--;
			outputs[MEM_OUTPUT].setVoltage(mem);
		}
		else if (outputs[0].isConnected() || outputs[1].isConnected())
		{
			for (int i = 0; i < INPUTS_LEN; i++)
			{
				if (inputs[i].getVoltage() > V_THR)
				{
					if (!risen[i]) {
						mem += params[i].getValue();
						risen[i] = true;
					}
				}
				else {
					risen[i] = false;
				}
			}
			mem -= params[LEAK_PARAM].getValue() * args.sampleTime;
			mem = mem < V_RST ? V_RST: mem;

			bool pressed = params[FIRE_PARAM].getValue() && !old_pressed;
			old_pressed = params[FIRE_PARAM].getValue();
			if (mem > V_THR || pressed) // SPIKE!
			{
				mem = V_RST;
				outputs[MEM_OUTPUT].setVoltage(V_ACT);
				ref_timer = params[REF_PARAM].getValue() * args.sampleRate;
				deqlay.push_back(params[DELAY_PARAM].getValue() * args.sampleRate);
			}
			else
			{
				outputs[MEM_OUTPUT].setVoltage(mem);
			}
		}
	}
};


struct LIFWidget : ModuleWidget {
	LIFWidget(LIF* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/LIF.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 36.0)), module, LIF::W1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(32.5, 36.0)), module, LIF::W2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(52.5, 36.0)), module, LIF::W3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(72.5, 36.0)), module, LIF::W4_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(92.5, 36.0)), module, LIF::W5_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 66.0)), module, LIF::W6_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(32.5, 66.0)), module, LIF::W7_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(52.5, 66.0)), module, LIF::W8_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(72.5, 66.0)), module, LIF::W9_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(92.5, 66.0)), module, LIF::W10_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(22.5, 106.0)), module, LIF::LEAK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 106.0)), module, LIF::REF_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(62.5, 106.0)), module, LIF::DELAY_PARAM));
		addParam(createParamCentered<BefacoPush>(mm2px(Vec(82.5, 106.0)), module, LIF::FIRE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 46.0)), module, LIF::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.5, 46.0)), module, LIF::IN2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(52.5, 46.0)), module, LIF::IN3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(72.5, 46.0)), module, LIF::IN4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.5, 46.0)), module, LIF::IN5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 76.0)), module, LIF::IN6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.5, 76.0)), module, LIF::IN7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(52.5, 76.0)), module, LIF::IN8_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(72.5, 76.0)), module, LIF::IN9_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.5, 76.0)), module, LIF::IN10_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.5, 101.0)), module, LIF::MEM_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.5, 111.0)), module, LIF::FIRE_OUTPUT));
	}
};


Model* modelLIF = createModel<LIF, LIFWidget>("LIF");