#include <algorithm>
#include <complex>

#include "netxpto.h"
#include "photodiode.h"


void Photodiode::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[0]->centralWavelength = outputOpticalWavelength;
	outputSignals[0]->centralFrequency = outputOpticalFrequency;

}



bool Photodiode::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	/*t_real radius = 0.0003; // radius of sensor
	t_real E0 = 8.854187817e-12;
	t_real n = 1.1;*/

	t_complex inputSignal1;
	t_complex inputSignal2;

	for (int i = 0; i < process; i++) {

			inputSignals[0]->bufferGet(&inputSignal1);
			inputSignals[1]->bufferGet(&inputSignal2);

			t_real power1 = abs(inputSignal1)*abs(inputSignal1)*2; 
			t_real current1 = responsivity * power1;

			t_real power2 = abs(inputSignal2)*abs(inputSignal2)*2;
			t_real current2 = responsivity * power2;

			t_real outputSignal = current1 - current2;

			outputSignals[0]->bufferPut(outputSignal);

		
	}
	return true;
}