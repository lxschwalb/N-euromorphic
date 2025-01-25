#include "plugin.hpp"
#include "NeuronConstants.hpp"
#include <algorithm>
#include <cmath>

const float notefreqs[74] = {25.956544, 27.500000, 29.135235, 30.867706, 32.703196, 34.647829, 36.708096, 38.890873, 41.203445, 43.653529, 46.249303, 48.999429, 51.913087, 55.000000, 58.270470, 61.735413, 65.406391, 69.295658, 73.416192, 77.781746, 82.406889, 87.307058, 92.498606, 97.998859, 103.826174, 110.000000, 116.540940, 123.470825, 130.812783, 138.591315, 146.832384, 155.563492, 164.813778, 174.614116, 184.997211, 195.997718, 207.652349, 220.000000, 233.081881, 246.941651, 261.625565, 277.182631, 293.664768, 311.126984, 329.627557, 349.228231, 369.994423, 391.995436, 415.304698, 440.000000, 466.163762, 493.883301, 523.251131, 554.365262, 587.329536, 622.253967, 659.255114, 698.456463, 739.988845, 783.990872, 830.609395, 880.000000, 932.327523, 987.766603, 1046.502261, 1108.730524, 1174.659072, 1244.507935, 1318.510228, 1396.912926, 1479.977691, 1567.981744, 1661.218790, 1760.000000};

struct Cochlea : Module {
	enum ParamId {
		REF_PARAM,
		RESP_PARAM,
		SENS_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		AUDIO_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		A0_OUTPUT,
		BB0_OUTPUT,
		B0_OUTPUT,
		C0_OUTPUT,
		DB0_OUTPUT,
		D0_OUTPUT,
		EB0_OUTPUT,
		E0_OUTPUT,
		F0_OUTPUT,
		GB0_OUTPUT,
		G0_OUTPUT,
		AB0_OUTPUT,
		A1_OUTPUT,
		BB1_OUTPUT,
		B1_OUTPUT,
		C1_OUTPUT,
		DB1_OUTPUT,
		D1_OUTPUT,
		EB1_OUTPUT,
		E1_OUTPUT,
		F1_OUTPUT,
		GB1_OUTPUT,
		G1_OUTPUT,
		AB1_OUTPUT,
		A2_OUTPUT,
		BB2_OUTPUT,
		B2_OUTPUT,
		C2_OUTPUT,
		DB2_OUTPUT,
		D2_OUTPUT,
		EB2_OUTPUT,
		E2_OUTPUT,
		F2_OUTPUT,
		GB2_OUTPUT,
		G2_OUTPUT,
		AB2_OUTPUT,		
		A3_OUTPUT,
		BB3_OUTPUT,
		B3_OUTPUT,
		C3_OUTPUT,
		DB3_OUTPUT,
		D3_OUTPUT,
		EB3_OUTPUT,
		E3_OUTPUT,
		F3_OUTPUT,
		GB3_OUTPUT,
		G3_OUTPUT,
		AB3_OUTPUT,
		A4_OUTPUT,
		BB4_OUTPUT,
		B4_OUTPUT,
		C4_OUTPUT,
		DB4_OUTPUT,
		D4_OUTPUT,
		EB4_OUTPUT,
		E4_OUTPUT,
		F4_OUTPUT,
		GB4_OUTPUT,
		G4_OUTPUT,
		AB4_OUTPUT,
		A5_OUTPUT,
		BB5_OUTPUT,
		B5_OUTPUT,
		C5_OUTPUT,
		DB5_OUTPUT,
		D5_OUTPUT,
		EB5_OUTPUT,
		E5_OUTPUT,
		F5_OUTPUT,
		GB5_OUTPUT,
		G5_OUTPUT,
		AB5_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	static constexpr int BUFFER_LEN = 4096;

	alignas(16) float inputBuffer[BUFFER_LEN] = {};
	alignas(16) float fftBuffer[BUFFER_LEN * 2];
	alignas(16) float specBuffer[BUFFER_LEN] = {};
	float mems[OUTPUTS_LEN] = {};
	int frame = 0;
	dsp::RealFFT fft;
	int subsamplecounter = 0;
	dsp::PulseGenerator fire[OUTPUTS_LEN];

	Cochlea() : fft(BUFFER_LEN) {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(REF_PARAM, 0.f, 1.f, 0.f, "Refractory Period");
		configParam(RESP_PARAM, 0.f, 5.f, 0.f, "Responsiveness");
		configParam(SENS_PARAM, 0.f, 1.f, 0.5f, "Sensitivity");
		configInput(AUDIO_INPUT, "Audio In");
		configOutput(A5_OUTPUT, "A5");
		configOutput(BB5_OUTPUT, "Bb5");
		configOutput(B5_OUTPUT, "B5");
		configOutput(C5_OUTPUT, "C5");
		configOutput(DB5_OUTPUT, "Db5");
		configOutput(D5_OUTPUT, "D5");
		configOutput(EB5_OUTPUT, "Eb5");
		configOutput(E5_OUTPUT, "E5");
		configOutput(F5_OUTPUT, "F5");
		configOutput(GB5_OUTPUT, "Gb5");
		configOutput(G5_OUTPUT, "G5");
		configOutput(AB5_OUTPUT, "Ab5");
		configOutput(A4_OUTPUT, "A4");
		configOutput(BB4_OUTPUT, "Bb4");
		configOutput(B4_OUTPUT, "B4");
		configOutput(C4_OUTPUT, "C4");
		configOutput(DB4_OUTPUT, "Db4");
		configOutput(D4_OUTPUT, "D4");
		configOutput(EB4_OUTPUT, "Eb4");
		configOutput(E4_OUTPUT, "E4");
		configOutput(F4_OUTPUT, "F4");
		configOutput(GB4_OUTPUT, "Gb4");
		configOutput(G4_OUTPUT, "G4");
		configOutput(AB4_OUTPUT, "Ab4");
		configOutput(A3_OUTPUT, "A3");
		configOutput(BB3_OUTPUT, "Bb3");
		configOutput(B3_OUTPUT, "B3");
		configOutput(C3_OUTPUT, "C3");
		configOutput(DB3_OUTPUT, "Db3");
		configOutput(D3_OUTPUT, "D3");
		configOutput(EB3_OUTPUT, "Eb3");
		configOutput(E3_OUTPUT, "E3");
		configOutput(F3_OUTPUT, "F3");
		configOutput(GB3_OUTPUT, "Gb3");
		configOutput(G3_OUTPUT, "G3");
		configOutput(AB3_OUTPUT, "Ab3");
		configOutput(A2_OUTPUT, "A2");
		configOutput(BB2_OUTPUT, "Bb2");
		configOutput(B2_OUTPUT, "B2");
		configOutput(C2_OUTPUT, "C2");
		configOutput(DB2_OUTPUT, "Db2");
		configOutput(D2_OUTPUT, "D2");
		configOutput(EB2_OUTPUT, "Eb2");
		configOutput(E2_OUTPUT, "E2");
		configOutput(F2_OUTPUT, "F2");
		configOutput(GB2_OUTPUT, "Gb2");
		configOutput(G2_OUTPUT, "G2");
		configOutput(AB2_OUTPUT, "Ab2");
		configOutput(A1_OUTPUT, "A1");
		configOutput(BB1_OUTPUT, "Bb1");
		configOutput(B1_OUTPUT, "B1");
		configOutput(C1_OUTPUT, "C1");
		configOutput(DB1_OUTPUT, "Db1");
		configOutput(D1_OUTPUT, "D1");
		configOutput(EB1_OUTPUT, "Eb1");
		configOutput(E1_OUTPUT, "E1");
		configOutput(F1_OUTPUT, "F1");
		configOutput(GB1_OUTPUT, "Gb1");
		configOutput(G1_OUTPUT, "G1");
		configOutput(AB1_OUTPUT, "Ab1");
		configOutput(A0_OUTPUT, "A0");
		configOutput(BB0_OUTPUT, "Bb0");
		configOutput(B0_OUTPUT, "B0");
		configOutput(C0_OUTPUT, "C0");
		configOutput(DB0_OUTPUT, "Db0");
		configOutput(D0_OUTPUT, "D0");
		configOutput(EB0_OUTPUT, "Eb0");
		configOutput(E0_OUTPUT, "E0");
		configOutput(F0_OUTPUT, "F0");
		configOutput(GB0_OUTPUT, "Gb0");
		configOutput(G0_OUTPUT, "G0");
		configOutput(AB0_OUTPUT, "Ab0");
	}

	void process(const ProcessArgs& args) override {
		for (int i = 0; i < OUTPUTS_LEN; i++)
			if(outputs[i].isConnected())
				outputs[i].setVoltage(fire[i].process(args.sampleTime) ? SPIKE_MAG : V_RST);

		if (++subsamplecounter >= 8)
		{
			subsamplecounter=0;
		
			inputBuffer[frame] = inputs[AUDIO_INPUT].getVoltage();
			frame = (frame+1)%BUFFER_LEN;
			
			fft.rfft(inputBuffer, fftBuffer);
			
			for (int i = 0; i < BUFFER_LEN; i++)
				specBuffer[i]=sqrt(fftBuffer[i*2]*fftBuffer[i*2]+fftBuffer[i*2+1]*fftBuffer[i*2+1]);

			for (int i = -1; i < OUTPUTS_LEN+1; i++)
			{
				float noteFreqIdx = notefreqs[i+1]/(args.sampleRate/8/BUFFER_LEN);
				float noteEnergy = math::interpolateLinear(specBuffer, noteFreqIdx)*params[SENS_PARAM].getValue();
				if (i>=0 && i<OUTPUTS_LEN)
				{
					mems[i] += noteEnergy*2e-6;
				}
				if(i>0){
					mems[i-1] -= noteEnergy*1e-6;
				}
				if(i<OUTPUTS_LEN-1){
					mems[i+1] -= noteEnergy*1e-6;
				}
			}
			
			for (int i = 0; i < OUTPUTS_LEN; i++)
				if(outputs[i].isConnected()){
					mems[i] -= params[RESP_PARAM].getValue() * args.sampleTime * 32;
					mems[i] = mems[i] < V_RST ? V_RST: mems[i];
					if (mems[i] > V_THR) // SPIKE!
					{
						mems[i] = V_RST;
						fire[i].trigger(1e-3);
					}
				}
		}
	}
};

struct CochleaWidget : ModuleWidget {
	CochleaWidget(Cochlea* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Cochlea.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(18.5, 96.0)), module, Cochlea::REF_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(18.5, 106.0)), module, Cochlea::RESP_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(18.5, 116.0)), module, Cochlea::SENS_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 106.0)), module, Cochlea::AUDIO_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 31.0)), module, Cochlea::A5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 31.0)), module, Cochlea::BB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 31.0)), module, Cochlea::B5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 31.0)), module, Cochlea::C5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 31.0)), module, Cochlea::DB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 31.0)), module, Cochlea::D5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 31.0)), module, Cochlea::EB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 31.0)), module, Cochlea::E5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 31.0)), module, Cochlea::F5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 31.0)), module, Cochlea::GB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 31.0)), module, Cochlea::G5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 31.0)), module, Cochlea::AB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 41.0)), module, Cochlea::A4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 41.0)), module, Cochlea::BB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 41.0)), module, Cochlea::B4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 41.0)), module, Cochlea::C4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 41.0)), module, Cochlea::DB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 41.0)), module, Cochlea::D4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 41.0)), module, Cochlea::EB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 41.0)), module, Cochlea::E4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 41.0)), module, Cochlea::F4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 41.0)), module, Cochlea::GB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 41.0)), module, Cochlea::G4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 41.0)), module, Cochlea::AB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 51.0)), module, Cochlea::A3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 51.0)), module, Cochlea::BB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 51.0)), module, Cochlea::B3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 51.0)), module, Cochlea::C3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 51.0)), module, Cochlea::DB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 51.0)), module, Cochlea::D3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 51.0)), module, Cochlea::EB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 51.0)), module, Cochlea::E3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 51.0)), module, Cochlea::F3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 51.0)), module, Cochlea::GB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 51.0)), module, Cochlea::G3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 51.0)), module, Cochlea::AB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 61.0)), module, Cochlea::A2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 61.0)), module, Cochlea::BB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 61.0)), module, Cochlea::B2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 61.0)), module, Cochlea::C2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 61.0)), module, Cochlea::DB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 61.0)), module, Cochlea::D2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 61.0)), module, Cochlea::EB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 61.0)), module, Cochlea::E2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 61.0)), module, Cochlea::F2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 61.0)), module, Cochlea::GB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 61.0)), module, Cochlea::G2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 61.0)), module, Cochlea::AB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 71.0)), module, Cochlea::A1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 71.0)), module, Cochlea::BB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 71.0)), module, Cochlea::B1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 71.0)), module, Cochlea::C1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 71.0)), module, Cochlea::DB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 71.0)), module, Cochlea::D1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 71.0)), module, Cochlea::EB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 71.0)), module, Cochlea::E1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 71.0)), module, Cochlea::F1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 71.0)), module, Cochlea::GB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 71.0)), module, Cochlea::G1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 71.0)), module, Cochlea::AB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 81.0)), module, Cochlea::A0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 81.0)), module, Cochlea::BB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 81.0)), module, Cochlea::B0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 81.0)), module, Cochlea::C0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 81.0)), module, Cochlea::DB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 81.0)), module, Cochlea::D0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 81.0)), module, Cochlea::EB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 81.0)), module, Cochlea::E0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 81.0)), module, Cochlea::F0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 81.0)), module, Cochlea::GB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 81.0)), module, Cochlea::G0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 81.0)), module, Cochlea::AB0_OUTPUT));
	}
};


Model* modelCochlea = createModel<Cochlea, CochleaWidget>("Cochlea");