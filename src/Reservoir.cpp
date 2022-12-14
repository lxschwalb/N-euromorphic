#include "plugin.hpp"


struct Reservoir : Module {
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
		PARAMS_LEN
	};
	enum InputId {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		IN5_INPUT,
		APPLY_INPUT,
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

	Reservoir() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(W_VAR_PARAM, 0.f, 1.f, 0.f, "");
		configParam(W_AVG_PARAM, 0.f, 1.f, 0.f, "");
		configParam(NUM_NEURONS_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DIST_PARAM, 0.f, 1.f, 0.f, "");
		configParam(L_VAR_PARAM, 0.f, 1.f, 0.f, "");
		configParam(L_AVG_PARAM, 0.f, 1.f, 0.f, "");
		configParam(R_VAR_PARAM, 0.f, 1.f, 0.f, "");
		configParam(R_AVG_PARAM, 0.f, 1.f, 0.f, "");
		configParam(APPLY_PARAM, 0.f, 1.f, 0.f, "");
		configParam(D_VAR_PARAM, 0.f, 1.f, 0.f, "");
		configParam(D_AVG_PARAM, 0.f, 1.f, 0.f, "");
		configInput(IN1_INPUT, "");
		configInput(IN2_INPUT, "");
		configInput(IN3_INPUT, "");
		configInput(IN4_INPUT, "");
		configInput(IN5_INPUT, "");
		configInput(APPLY_INPUT, "");
		configInput(IN6_INPUT, "");
		configInput(IN7_INPUT, "");
		configInput(IN8_INPUT, "");
		configOutput(OUT1_OUTPUT, "");
		configOutput(OUT2_OUTPUT, "");
		configOutput(OUT3_OUTPUT, "");
		configOutput(OUT4_OUTPUT, "");
		configOutput(OUT5_OUTPUT, "");
		configOutput(OUT6_OUTPUT, "");
		configOutput(OUT7_OUTPUT, "");
		configOutput(OUT8_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct ReservoirWidget : ModuleWidget {
	ReservoirWidget(Reservoir* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Reservoir.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 118.0)), module, Reservoir::W_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.5, 108.0)), module, Reservoir::W_AVG_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(37.5, 33.5)), module, Reservoir::NUM_NEURONS_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(63.5, 33.5)), module, Reservoir::DIST_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 118.0)), module, Reservoir::L_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(40.5, 108.0)), module, Reservoir::L_AVG_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 118.0)), module, Reservoir::R_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(60.5, 108.0)), module, Reservoir::R_AVG_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(50.5, 63.5)), module, Reservoir::APPLY_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 118.0)), module, Reservoir::D_VAR_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(80.5, 108.0)), module, Reservoir::D_AVG_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 28.5)), module, Reservoir::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 38.5)), module, Reservoir::IN2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 48.5)), module, Reservoir::IN3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 58.5)), module, Reservoir::IN4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 68.5)), module, Reservoir::IN5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.5, 73.5)), module, Reservoir::APPLY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 78.5)), module, Reservoir::IN6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 88.5)), module, Reservoir::IN7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.5, 98.5)), module, Reservoir::IN8_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 28.5)), module, Reservoir::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 38.5)), module, Reservoir::OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 48.5)), module, Reservoir::OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 58.5)), module, Reservoir::OUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 68.5)), module, Reservoir::OUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 78.5)), module, Reservoir::OUT6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 88.5)), module, Reservoir::OUT7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(90.5, 98.5)), module, Reservoir::OUT8_OUTPUT));
	}
};


Model* modelReservoir = createModel<Reservoir, ReservoirWidget>("Reservoir");