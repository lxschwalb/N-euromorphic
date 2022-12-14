#include "plugin.hpp"
#include "NeuronConstants.hpp"

struct CV2ISI : Module {
	enum ParamId {
		W11_PARAM,
		W12_PARAM,
		W13_PARAM,
		W21_PARAM,
		W22_PARAM,
		W23_PARAM,
		W31_PARAM,
		W32_PARAM,
		W33_PARAM,
		W41_PARAM,
		W42_PARAM,
		W43_PARAM,
		MODE1_PARAM,
		MODE2_PARAM,
		MODE3_PARAM,
		MODE4_PARAM,
		FIRE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		IN11_INPUT,
		IN12_INPUT,
		IN13_INPUT,
		IN21_INPUT,
		IN22_INPUT,
		IN23_INPUT,
		IN31_INPUT,
		IN32_INPUT,
		IN33_INPUT,
		IN41_INPUT,
		IN42_INPUT,
		IN43_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		MEM1_OUTPUT,
		FIRE1_OUTPUT,
		MEM2_OUTPUT,
		FIRE2_OUTPUT,
		MEM3_OUTPUT,
		FIRE3_OUTPUT,
		MEM4_OUTPUT,
		FIRE4_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	float mem[4] = {0, 0, 0, 0};
	float rise = V_THR - V_RST;
	dsp::PulseGenerator fire[4];
	bool old_pressed = false;

	CV2ISI() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(W11_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W12_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W13_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W21_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W22_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W23_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W31_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W32_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W33_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W41_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W42_PARAM, -1.f, 1.f, 0.f, "");
		configParam(W43_PARAM, -1.f, 1.f, 0.f, "");
		configParam(MODE1_PARAM, 0, 2, 0, "Mode 1", "", 0.f);
		configParam(MODE2_PARAM, 0, 2, 0, "Mode 2", "", 0.f);
		configParam(MODE3_PARAM, 0, 2, 0, "Mode 3", "", 0.f);
		configParam(MODE4_PARAM, 0, 2, 0, "Mode 4", "", 0.f);
		configParam(FIRE_PARAM, false, true, false, "FIRE");
		configInput(IN11_INPUT, "");
		configInput(IN12_INPUT, "");
		configInput(IN13_INPUT, "");
		configInput(IN21_INPUT, "");
		configInput(IN22_INPUT, "");
		configInput(IN23_INPUT, "");
		configInput(IN31_INPUT, "");
		configInput(IN32_INPUT, "");
		configInput(IN33_INPUT, "");
		configInput(IN41_INPUT, "");
		configInput(IN42_INPUT, "");
		configInput(IN43_INPUT, "");
		configOutput(MEM1_OUTPUT, "Membrane 1");
		configOutput(FIRE1_OUTPUT, "Fire 1");
		configOutput(MEM2_OUTPUT, "Membrane 2");
		configOutput(FIRE2_OUTPUT, "Fire 2");
		configOutput(MEM3_OUTPUT, "Membrane 3");
		configOutput(FIRE3_OUTPUT, "Fire 3");
		configOutput(MEM4_OUTPUT, "Membrane 4");
		configOutput(FIRE4_OUTPUT, "Fire 4");
	}

	void process(const ProcessArgs& args) override {
		// MODE 0: CV ISI
		// Mode 1: CV HZ
		// Mode 2: CV V/OCT
		for (int i = 0; i < 4; i++)
		{
			if(outputs[2*i].isConnected() || outputs[2*i + 1].isConnected())
			{
				float cv_sum = 0;
				for (int j = 0; j < 3; j++)
				{				
					cv_sum += inputs[i*3 + j].isConnected() ?
							inputs[i*3 + j].getVoltage()*params[i*3 + j].getValue() :
							params[i*3 + j].getValue();
				}
				outputs[2*i + 1].setVoltage(fire[i].process(args.sampleTime) ? SPIKE_MAG : V_RST);

				if (mem[i] > V_THR) // SPIKE!
				{
					mem[i] = V_RST;
					outputs[2*i].setVoltage(V_ACT);
					fire[i].trigger(1e-3);
				}
				else
				{
					outputs[2*i].setVoltage(mem[i]);
					switch ((int)params[i + 12].getValue())
					{
					case 0:
						mem[i] += cv_sum != 0 ?
								rise * args.sampleTime / cv_sum :
								0;
						break;
					case 1:
						mem[i] += rise * args.sampleTime * cv_sum;
						break;
					case 2:
						mem[i] += rise * args.sampleTime * middle_c * pow(2, cv_sum);
						break;
					
					default:
						break;
					}
					mem[i] = mem[i] < V_RST ? V_RST: mem[i];
				}
			}
		}

		bool pressed = params[FIRE_PARAM].getValue() && !old_pressed;
		old_pressed = params[FIRE_PARAM].getValue();
		if (pressed) // SPIKE!
		{
			for (int i = 0; i < 4; i++)
			{
				mem[i] = V_RST;
				outputs[2*i].setVoltage(V_ACT);
				fire[i].trigger(1e-3);
			}
			
		}
	}
};


struct CV2ISIWidget : ModuleWidget {
	CV2ISIWidget(CV2ISI* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/CV2ISI.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 31.0)), module, CV2ISI::W11_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 31.0)), module, CV2ISI::W12_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 31.0)), module, CV2ISI::W13_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 56.0)), module, CV2ISI::W21_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 56.0)), module, CV2ISI::W22_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 56.0)), module, CV2ISI::W23_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 81.0)), module, CV2ISI::W31_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 81.0)), module, CV2ISI::W32_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 81.0)), module, CV2ISI::W33_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 106.0)), module, CV2ISI::W41_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 106.0)), module, CV2ISI::W42_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 106.0)), module, CV2ISI::W43_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(57.5, 36.0)), module, CV2ISI::MODE1_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(57.5, 61.0)), module, CV2ISI::MODE2_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(57.5, 86.0)), module, CV2ISI::MODE3_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(57.5, 111.0)), module, CV2ISI::MODE4_PARAM));
		addParam(createParamCentered<BefacoPush>(mm2px(Vec(80.0, 121.0)), module, CV2ISI::FIRE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 41.0)), module, CV2ISI::IN11_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 41.0)), module, CV2ISI::IN12_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 41.0)), module, CV2ISI::IN13_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 66.0)), module, CV2ISI::IN21_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 66.0)), module, CV2ISI::IN22_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 66.0)), module, CV2ISI::IN23_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 91.0)), module, CV2ISI::IN31_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 91.0)), module, CV2ISI::IN32_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 91.0)), module, CV2ISI::IN33_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 116.0)), module, CV2ISI::IN41_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 116.0)), module, CV2ISI::IN42_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 116.0)), module, CV2ISI::IN43_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 36.0)), module, CV2ISI::MEM1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 36.0)), module, CV2ISI::FIRE1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 61.0)), module, CV2ISI::MEM2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 61.0)), module, CV2ISI::FIRE2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 86.0)), module, CV2ISI::MEM3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 86.0)), module, CV2ISI::FIRE3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 111.0)), module, CV2ISI::MEM4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 111.0)), module, CV2ISI::FIRE4_OUTPUT));
	}
};


Model* modelCV2ISI = createModel<CV2ISI, CV2ISIWidget>("CV2ISI");