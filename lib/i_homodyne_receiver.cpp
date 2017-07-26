#include "i_homodyne_receiver.h"


I_HomodyneReceiver::I_HomodyneReceiver(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	inputSignals = inputSignal;
	outputSignals = outputSignal;

	B1.initializeBlock(vector<Signal*> { inputSignals[0], inputSignals[1] }, vector<Signal*> { &I_HMD01 });
	B2.initializeBlock(vector<Signal*> { &I_HMD01 }, vector<Signal*> { &I_HMD02 });

	setModuleBlocks({ &B1, &B2 });
};

