#include "coincidence_detector_20180206.h"

void CoincidenceDetector::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[1]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
}

bool CoincidenceDetector::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {
		t_binary value0;
		inputSignals[0]->bufferGet(&value0);

		t_binary value1;
		inputSignals[1]->bufferGet(&value1);

		if ((value0 == 1) && (value1 == 0)) {
			outputSignals[0]->bufferPut((t_real)0.0);
		}
		else if ((value0 == 0) && (value1 == 1)) {
			outputSignals[0]->bufferPut((t_real)1.0);
		}
		else if ((value0 == 1) && (value1 == 1)) {
			outputSignals[0]->bufferPut((t_real)-2.0);
		}
		else {
			outputSignals[0]->bufferPut((t_real)-1.0);
		}
		
	}

	return true;
};
