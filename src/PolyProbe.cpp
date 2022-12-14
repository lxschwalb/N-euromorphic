#include "plugin.hpp"
#include "NeuronConstants.hpp"
#include <algorithm>


struct PolyProbe : Module {
	enum ParamId {
		POLY_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		A0_INPUT,
		BB0_INPUT,
		B0_INPUT,
		C0_INPUT,
		DB0_INPUT,
		D0_INPUT,
		EB0_INPUT,
		E0_INPUT,
		F0_INPUT,
		GB0_INPUT,
		G0_INPUT,
		AB0_INPUT,
		A1_INPUT,
		BB1_INPUT,
		B1_INPUT,
		C1_INPUT,
		DB1_INPUT,
		D1_INPUT,
		EB1_INPUT,
		E1_INPUT,
		F1_INPUT,
		GB1_INPUT,
		G1_INPUT,
		AB1_INPUT,
		A2_INPUT,
		BB2_INPUT,
		B2_INPUT,
		C2_INPUT,
		DB2_INPUT,
		D2_INPUT,
		EB2_INPUT,
		E2_INPUT,
		F2_INPUT,
		GB2_INPUT,
		G2_INPUT,
		AB2_INPUT,
		A3_INPUT,
		BB3_INPUT,
		B3_INPUT,
		C3_INPUT,
		DB3_INPUT,
		D3_INPUT,
		EB3_INPUT,
		E3_INPUT,
		F3_INPUT,
		GB3_INPUT,
		G3_INPUT,
		AB3_INPUT,
		A4_INPUT,
		BB4_INPUT,
		B4_INPUT,
		C4_INPUT,
		DB4_INPUT,
		D4_INPUT,
		EB4_INPUT,
		E4_INPUT,
		F4_INPUT,
		GB4_INPUT,
		G4_INPUT,
		AB4_INPUT,
		A5_INPUT,
		BB5_INPUT,
		B5_INPUT,
		C5_INPUT,
		DB5_INPUT,
		D5_INPUT,
		EB5_INPUT,
		E5_INPUT,
		F5_INPUT,
		GB5_INPUT,
		G5_INPUT,
		AB5_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		V_OCT_OUTPUT,
		TRIG_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	int poly_track = 0;
	bool risen[INPUTS_LEN] = {false};
	dsp::PulseGenerator trig;

	PolyProbe() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(POLY_PARAM, 1, 16, 4, "Polyphony", "", 0.f);
		configInput(A5_INPUT, "A5");
		configInput(BB5_INPUT, "Bb5");
		configInput(B5_INPUT, "B5");
		configInput(C5_INPUT, "C5");
		configInput(DB5_INPUT, "Db5");
		configInput(D5_INPUT, "D5");
		configInput(EB5_INPUT, "Eb5");
		configInput(E5_INPUT, "E5");
		configInput(F5_INPUT, "F5");
		configInput(GB5_INPUT, "Gb5");
		configInput(G5_INPUT, "G5");
		configInput(AB5_INPUT, "Ab5");
		configInput(A4_INPUT, "A4");
		configInput(BB4_INPUT, "Bb4");
		configInput(B4_INPUT, "B4");
		configInput(C4_INPUT, "C4");
		configInput(DB4_INPUT, "Db4");
		configInput(D4_INPUT, "D4");
		configInput(EB4_INPUT, "Eb4");
		configInput(E4_INPUT, "E4");
		configInput(F4_INPUT, "F4");
		configInput(GB4_INPUT, "Gb4");
		configInput(G4_INPUT, "G4");
		configInput(AB4_INPUT, "Ab4");
		configInput(A3_INPUT, "A3");
		configInput(BB3_INPUT, "Bb3");
		configInput(B3_INPUT, "B3");
		configInput(C3_INPUT, "C3");
		configInput(DB3_INPUT, "Db3");
		configInput(D3_INPUT, "D3");
		configInput(EB3_INPUT, "Eb3");
		configInput(E3_INPUT, "E3");
		configInput(F3_INPUT, "F3");
		configInput(GB3_INPUT, "Gb3");
		configInput(G3_INPUT, "G3");
		configInput(AB3_INPUT, "Ab3");
		configInput(A2_INPUT, "A2");
		configInput(BB2_INPUT, "Bb2");
		configInput(B2_INPUT, "B2");
		configInput(C2_INPUT, "C2");
		configInput(DB2_INPUT, "Db2");
		configInput(D2_INPUT, "D2");
		configInput(EB2_INPUT, "Eb2");
		configInput(E2_INPUT, "E2");
		configInput(F2_INPUT, "F2");
		configInput(GB2_INPUT, "Gb2");
		configInput(G2_INPUT, "G2");
		configInput(AB2_INPUT, "Ab2");
		configInput(A1_INPUT, "A1");
		configInput(BB1_INPUT, "Bb1");
		configInput(B1_INPUT, "B1");
		configInput(C1_INPUT, "C1");
		configInput(DB1_INPUT, "Db1");
		configInput(D1_INPUT, "D1");
		configInput(EB1_INPUT, "Eb1");
		configInput(E1_INPUT, "E1");
		configInput(F1_INPUT, "F1");
		configInput(GB1_INPUT, "Gb1");
		configInput(G1_INPUT, "G1");
		configInput(AB1_INPUT, "Ab1");
		configInput(A0_INPUT, "A0");
		configInput(BB0_INPUT, "Bb0");
		configInput(B0_INPUT, "B0");
		configInput(C0_INPUT, "C0");
		configInput(DB0_INPUT, "Db0");
		configInput(D0_INPUT, "D0");
		configInput(EB0_INPUT, "Eb0");
		configInput(E0_INPUT, "E0");
		configInput(F0_INPUT, "F0");
		configInput(GB0_INPUT, "Gb0");
		configInput(G0_INPUT, "G0");
		configInput(AB0_INPUT, "Ab0");
		configOutput(V_OCT_OUTPUT, "Pitch");
		configOutput(TRIG_OUTPUT, "Trigger");
	}

	void process(const ProcessArgs& args) override {
		int num_channels = params[POLY_PARAM].getValue();
		outputs[TRIG_OUTPUT].setVoltage(trig.process(args.sampleTime) ? 10.f : 0.f, poly_track);

		for (int i = 0; i < INPUTS_LEN; i++)
		{
			if(inputs[i].isConnected())
			{
				if (inputs[i].getVoltage() > V_THR)
				{
					if (!risen[i]) {
						outputs[V_OCT_OUTPUT].setVoltage((i-27)/12.f, poly_track);
						trig.trigger(1e-3);
						poly_track++;
						poly_track %= num_channels;
						risen[i] = true;
					}
				}
				else {
					risen[i] = false;
				}
			}
		}

		outputs[V_OCT_OUTPUT].setChannels(num_channels);
		outputs[TRIG_OUTPUT].setChannels(num_channels);
	}
};


struct PolyProbeWidget : ModuleWidget {
	PolyProbeWidget(PolyProbe* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/PolyProbe.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(78.5, 106.0)), module, PolyProbe::POLY_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 31.0)), module, PolyProbe::A5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.5, 31.0)), module, PolyProbe::BB5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.5, 31.0)), module, PolyProbe::B5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.5, 31.0)), module, PolyProbe::C5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.5, 31.0)), module, PolyProbe::DB5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.5, 31.0)), module, PolyProbe::D5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(54.5, 31.0)), module, PolyProbe::EB5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.5, 31.0)), module, PolyProbe::E5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.5, 31.0)), module, PolyProbe::F5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(78.5, 31.0)), module, PolyProbe::GB5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(86.5, 31.0)), module, PolyProbe::G5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(94.5, 31.0)), module, PolyProbe::AB5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 41.0)), module, PolyProbe::A4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.5, 41.0)), module, PolyProbe::BB4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.5, 41.0)), module, PolyProbe::B4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.5, 41.0)), module, PolyProbe::C4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.5, 41.0)), module, PolyProbe::DB4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.5, 41.0)), module, PolyProbe::D4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(54.5, 41.0)), module, PolyProbe::EB4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.5, 41.0)), module, PolyProbe::E4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.5, 41.0)), module, PolyProbe::F4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(78.5, 41.0)), module, PolyProbe::GB4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(86.5, 41.0)), module, PolyProbe::G4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(94.5, 41.0)), module, PolyProbe::AB4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 51.0)), module, PolyProbe::A3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.5, 51.0)), module, PolyProbe::BB3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.5, 51.0)), module, PolyProbe::B3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.5, 51.0)), module, PolyProbe::C3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.5, 51.0)), module, PolyProbe::DB3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.5, 51.0)), module, PolyProbe::D3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(54.5, 51.0)), module, PolyProbe::EB3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.5, 51.0)), module, PolyProbe::E3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.5, 51.0)), module, PolyProbe::F3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(78.5, 51.0)), module, PolyProbe::GB3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(86.5, 51.0)), module, PolyProbe::G3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(94.5, 51.0)), module, PolyProbe::AB3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 61.0)), module, PolyProbe::A2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.5, 61.0)), module, PolyProbe::BB2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.5, 61.0)), module, PolyProbe::B2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.5, 61.0)), module, PolyProbe::C2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.5, 61.0)), module, PolyProbe::DB2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.5, 61.0)), module, PolyProbe::D2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(54.5, 61.0)), module, PolyProbe::EB2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.5, 61.0)), module, PolyProbe::E2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.5, 61.0)), module, PolyProbe::F2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(78.5, 61.0)), module, PolyProbe::GB2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(86.5, 61.0)), module, PolyProbe::G2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(94.5, 61.0)), module, PolyProbe::AB2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 71.0)), module, PolyProbe::A1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.5, 71.0)), module, PolyProbe::BB1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.5, 71.0)), module, PolyProbe::B1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.5, 71.0)), module, PolyProbe::C1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.5, 71.0)), module, PolyProbe::DB1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.5, 71.0)), module, PolyProbe::D1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(54.5, 71.0)), module, PolyProbe::EB1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.5, 71.0)), module, PolyProbe::E1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.5, 71.0)), module, PolyProbe::F1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(78.5, 71.0)), module, PolyProbe::GB1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(86.5, 71.0)), module, PolyProbe::G1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(94.5, 71.0)), module, PolyProbe::AB1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 81.0)), module, PolyProbe::A0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.5, 81.0)), module, PolyProbe::BB0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.5, 81.0)), module, PolyProbe::B0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.5, 81.0)), module, PolyProbe::C0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(38.5, 81.0)), module, PolyProbe::DB0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.5, 81.0)), module, PolyProbe::D0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(54.5, 81.0)), module, PolyProbe::EB0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.5, 81.0)), module, PolyProbe::E0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.5, 81.0)), module, PolyProbe::F0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(78.5, 81.0)), module, PolyProbe::GB0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(86.5, 81.0)), module, PolyProbe::G0_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(94.5, 81.0)), module, PolyProbe::AB0_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.5, 101.0)), module, PolyProbe::V_OCT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.5, 111.0)), module, PolyProbe::TRIG_OUTPUT));
	}
};


Model* modelPolyProbe = createModel<PolyProbe, PolyProbeWidget>("PolyProbe");