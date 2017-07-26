#include <algorithm>
#include <complex>
#include <iostream>

#include "netxpto.h"
#include "sampler.h"


void Sampler::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSymbolPeriod());

}


bool Sampler::runBlock(void) {

	numberOfInputSignals = inputSignals.size();

	//Sampler with only one input signal (internal clock)
	if (numberOfInputSignals == 1) {

		int ready = inputSignals[0]->ready();

		if (samplesToSkip > 0) {
			int process = min(ready, samplesToSkip);

			for (int k = 0; k < process; k++) {
				t_real in;
				inputSignals[0]->bufferGet(&in);
			}

			samplesToSkip = samplesToSkip - process;

			if (samplesToSkip > 0) return true;

			ready = inputSignals[0]->ready();

		}

		int space = outputSignals[0]->space();
		int process = min(ready, space);


		if (process == 0) return false;

		int sPerSymbol = (int)(inputSignals[0]->getSamplesPerSymbol());


		for (int k = 0; k < process; k++) {
			t_real in;
			inputSignals[0]->bufferGet(&in);
			if (k % sPerSymbol == 0) {
				outputSignals[0]->bufferPut((t_real)in);
			}
		}

		return true;
	}

	//Sampler with two input signals (external clock)
	else {

		int ready1 = inputSignals[0]->ready();
		int ready2 = inputSignals[1]->ready();

		int ready = min(ready1, ready2);

		if (samplesToSkip > 0) {

			int process = min(ready, samplesToSkip);

			for (int k = 0; k < process; k++) {
				t_real in;
				inputSignals[0]->bufferGet(&in);
			}

			samplesToSkip = samplesToSkip - process;

			if (samplesToSkip > 0) return true;

			ready1 = inputSignals[0]->ready();
			ready2 = inputSignals[1]->ready();

			ready = min(ready1, ready2);

		}

		int space = outputSignals[0]->space();
		int process = min(ready, space);


		if (process == 0) return false;

		/*int ready1 = inputSignals[0]->ready();
		int ready2 = inputSignals[1]->ready();

		int ready = min(ready1, ready2);

		int space = outputSignals[0]->space();

		int process = min(ready, space);

		if (process == 0) return false;*/

		t_real inClock;
		t_real inSignal;

		for (int k = 0; k < process; k++) {

			inputSignals[1]->bufferGet(&inClock);
			inputSignals[0]->bufferGet(&inSignal);

			if (inClock == 1.0) {

				inSignal = inSignal; // (.5*sqrt(outputOpticalPower)); // to normalize the signal to 1
				outputSignals[0]->bufferPut(inSignal);

			}
		}

	}

	return 0;

};