#include "dsp_20180423.h"

DSP::DSP(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	setCPESamplingPeriod(inputSignal[0]->getSamplingPeriod());

	inputSignals = inputSignal;
	outputSignals = outputSignal;

/*

	*/

	B01.initializeBlock(vector<Signal*> { inputSignals[0], inputSignals[1] }, vector<Signal*> { &DSP01 }); // real to complex
	B02.initializeBlock(vector<Signal*> { &DSP01 }, vector<Signal*> { &DSP02 }); // cpe
	
	
	setModuleBlocks({ &B01, &B02 });
};