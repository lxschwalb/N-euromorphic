#include "plugin.hpp"
#include "NeuronConstants.hpp"


struct ISI2CV : Module {
	enum ParamId {
		G1_PARAM,
		B1_PARAM,
		M1_PARAM,
		G2_PARAM,
		B2_PARAM,
		M2_PARAM,
		G3_PARAM,
		B3_PARAM,
		M3_PARAM,
		G4_PARAM,
		B4_PARAM,
		M4_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		I1_INPUT,
		I2_INPUT,
		I3_INPUT,
		I4_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		O1_OUTPUT,
		O2_OUTPUT,
		O3_OUTPUT,
		O4_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	bool risen[INPUTS_LEN] = {false};
	int timers[INPUTS_LEN] = {0};
	float volt = 0;

	ISI2CV() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(G1_PARAM, -1.f, 1.f, 0.f, "Gain 1");
		configParam(B1_PARAM, -10.f, 10.f, 0.f, "Bias 1");
		configParam(M1_PARAM, 0, 2, 0, "Mode 1", "", 0.f);
		configParam(G2_PARAM, -1.f, 1.f, 0.f, "Gain 2");
		configParam(B2_PARAM, -10.f, 10.f, 0.f, "Bias 2");
		configParam(M2_PARAM, 0, 2, 0, "Mode 2", "", 0.f);
		configParam(G3_PARAM, -1.f, 1.f, 0.f, "Gain 3");
		configParam(B3_PARAM, -10.f, 10.f, 0.f, "Bias 3");
		configParam(M3_PARAM, 0, 2, 0, "Mode 3", "", 0.f);
		configParam(G4_PARAM, -1.f, 1.f, 0.f, "Gain 4");
		configParam(B4_PARAM, -10.f, 10.f, 0.f, "Bias 4");
		configParam(M4_PARAM, 0, 2, 0, "Mode 4", "", 0.f);
		configInput(I1_INPUT, "In 1");
		configInput(I2_INPUT, "In 2");
		configInput(I3_INPUT, "In 3");
		configInput(I4_INPUT, "In 4");
		configOutput(O1_OUTPUT, "Out 1");
		configOutput(O2_OUTPUT, "Out 2");
		configOutput(O3_OUTPUT, "Out 3");
		configOutput(O4_OUTPUT, "Out 4");
	}

	void process(const ProcessArgs& args) override {
		// Mode 0: CV ISI
		// Mode 1: CV HZ
		// Mode 2: CV V/OCT
		for (int i = 0; i < INPUTS_LEN; i++)
		{
			if(inputs[i].isConnected())
			{
				timers[i]++;
				if (inputs[i].getVoltage() > V_THR)
				{
					if (!risen[i]) {
						switch ((int)params[i*3 + 2].getValue())
						{
						case 0:
							volt = timers[i] * args.sampleTime;
							break;
						case 1:
							volt = 1/(timers[i] * args.sampleTime);
							break;
						case 2:
							volt = log2(1/(middle_c * timers[i] * args.sampleTime));
							break;
						
						default:
							break;
						}
						volt *= params[i*3].getValue();
						volt += params[i*3+1].getValue();
						volt = volt < 12 ? volt : 12;
						volt = volt > -12 ? volt : -12;
						risen[i] = true;
						timers[i] = 0;
					}
				}
				else
				{
					volt = outputs[i].getVoltage();
					risen[i] = false;
				}
			}
			outputs[i].setVoltage(volt);
		}
	}
};


struct ISI2CVWidget : ModuleWidget {
	ISI2CVWidget(ISI2CV* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/ISI2CV.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(32.5, 36.0)), module, ISI2CV::G1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(47.5, 36.0)), module, ISI2CV::B1_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(62.5, 36.0)), module, ISI2CV::M1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(32.5, 61.0)), module, ISI2CV::G2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(47.5, 61.0)), module, ISI2CV::B2_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(62.5, 61.0)), module, ISI2CV::M2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(32.5, 86.0)), module, ISI2CV::G3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(47.5, 86.0)), module, ISI2CV::B3_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(62.5, 86.0)), module, ISI2CV::M3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(32.5, 111.0)), module, ISI2CV::G4_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(47.5, 111.0)), module, ISI2CV::B4_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(62.5, 111.0)), module, ISI2CV::M4_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 36.0)), module, ISI2CV::I1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 61.0)), module, ISI2CV::I2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 86.0)), module, ISI2CV::I3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 111.0)), module, ISI2CV::I4_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(82.5, 36.0)), module, ISI2CV::O1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(82.5, 61.0)), module, ISI2CV::O2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(82.5, 86.0)), module, ISI2CV::O3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(82.5, 111.0)), module, ISI2CV::O4_OUTPUT));
	}
};


Model* modelISI2CV = createModel<ISI2CV, ISI2CVWidget>("ISI2CV");