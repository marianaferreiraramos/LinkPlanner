#include "bit_decision.h"

void BitDecision::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
//	outputSignals[0]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	outputSignals[0]->setSamplesPerSymbol(1);

	outputSignals[1]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
//	outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	outputSignals[1]->setSamplesPerSymbol(1);
}

bool BitDecision::runBlock() {
	bool alive{ false };

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();

	int ready = min(ready0, ready1);

	int space0 = outputSignals[0]->space();
	int space1 = outputSignals[1]->space();

	int space = min(space0, space1);

	int process = min(ready,space);

	if (process > 0) {
		for (auto n = 0; n < process; n++) {
			t_binary bitIn;
			t_binary logicValue;
			inputSignals[0]->bufferGet(&bitIn);
			inputSignals[1]->bufferGet(&logicValue);

			if (logicValue == 1) {
				outputSignals[0]->bufferPut((t_binary)bitIn);
			//	cout << "1: " << bitIn << "\n";
			}
			else {
				outputSignals[1]->bufferPut((t_binary)bitIn);
			//	cout << "0: " << bitIn << "\n";
			}
				
		}
		alive = true;
	}

	return alive;
}