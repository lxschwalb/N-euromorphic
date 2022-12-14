#include "plugin.hpp"


struct Temp : Module {
	enum ParamId {
		W11_PARAM,
		W12_PARAM,
		W13_PARAM,
		MODE1_PARAM,
		W21_PARAM,
		W22_PARAM,
		W23_PARAM,
		MODE2_PARAM,
		W31_PARAM,
		W32_PARAM,
		W33_PARAM,
		MODE3_PARAM,
		W41_PARAM,
		W42_PARAM,
		W43_PARAM,
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

	Temp() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(W11_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W12_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W13_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MODE1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W21_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W22_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W23_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MODE2_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W31_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W32_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W33_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MODE3_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W41_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W42_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W43_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MODE4_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FIRE_PARAM, 0.f, 1.f, 0.f, "");
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
		configOutput(MEM1_OUTPUT, "");
		configOutput(FIRE1_OUTPUT, "");
		configOutput(MEM2_OUTPUT, "");
		configOutput(FIRE2_OUTPUT, "");
		configOutput(MEM3_OUTPUT, "");
		configOutput(FIRE3_OUTPUT, "");
		configOutput(MEM4_OUTPUT, "");
		configOutput(FIRE4_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct TempWidget : ModuleWidget {
	TempWidget(Temp* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/temp.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 31.0)), module, Temp::W11_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 31.0)), module, Temp::W12_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 31.0)), module, Temp::W13_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(57.5, 36.0)), module, Temp::MODE1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 56.0)), module, Temp::W21_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 56.0)), module, Temp::W22_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 56.0)), module, Temp::W23_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(57.5, 61.0)), module, Temp::MODE2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 81.0)), module, Temp::W31_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 81.0)), module, Temp::W32_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 81.0)), module, Temp::W33_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(57.5, 86.0)), module, Temp::MODE3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.5, 106.0)), module, Temp::W41_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(27.5, 106.0)), module, Temp::W42_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(42.5, 106.0)), module, Temp::W43_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(57.5, 111.0)), module, Temp::MODE4_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(80.0, 121.0)), module, Temp::FIRE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 41.0)), module, Temp::IN11_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 41.0)), module, Temp::IN12_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 41.0)), module, Temp::IN13_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 66.0)), module, Temp::IN21_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 66.0)), module, Temp::IN22_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 66.0)), module, Temp::IN23_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 91.0)), module, Temp::IN31_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 91.0)), module, Temp::IN32_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 91.0)), module, Temp::IN33_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5, 116.0)), module, Temp::IN41_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.5, 116.0)), module, Temp::IN42_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.5, 116.0)), module, Temp::IN43_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 36.0)), module, Temp::MEM1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 36.0)), module, Temp::FIRE1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 61.0)), module, Temp::MEM2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 61.0)), module, Temp::FIRE2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 86.0)), module, Temp::MEM3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 86.0)), module, Temp::FIRE3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(75.0, 111.0)), module, Temp::MEM4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.0, 111.0)), module, Temp::FIRE4_OUTPUT));
	}
};


Model* modelTemp = createModel<Temp, TempWidget>("temp");