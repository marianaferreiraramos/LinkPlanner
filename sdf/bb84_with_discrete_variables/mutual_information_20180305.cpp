#include "mutual_information_20180305.h"

void MutualInformation::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
};

bool MutualInformation::runBlock(void) {
	bool alive{ false };

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();

	int ready = min(ready0, ready1);
	int space = outputSignals[0]->space();

	int process = min(ready,space);

	if (process > 0) {

		for (auto k = 0; k < process; k++) {
			t_binary bin0;
			inputSignals[0]->bufferGet(&bin0);
			t_binary bin1;
			inputSignals[1]->bufferGet(&bin1);

			if (bin1 == bin0) {
				outputSignals[0]->bufferPut((t_binary)1);
			}
			else
			{
				outputSignals[0]->bufferPut((t_binary)0);
			}
		}
		
	}

	return alive;
};

