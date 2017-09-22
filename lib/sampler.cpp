#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>

#include "netxpto.h"
#include "sampler.h"


void Sampler::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
}


bool Sampler::runBlock(void){


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

	ofstream myfile2;
	myfile2.open("translate.txt", fstream::app);

	int ready = inputSignals[0]->ready();

	int samplesPerSymbol = inputSignals[0]->getSamplesPerSymbol();
	
	if (firstPass || repeatedPass) 
	{

		if (firstPass)
		{
		samplesToSkip = samplesToSkip - process;
		repeatedPass = false;
		if (samplesToSkip != 0)
		{
			repeatedPass = true;
		}
		ready = inputSignals[0]->ready();
	}

	int space = outputSignals[0]->space();
	int process = min(ready, space);
	
	
	if (process == 0){
		myfile2.close();
		return false;
	}
	if (samplesToSkip == 0)
	{
		for (int k = 0; k < process; k++) {
			t_real in;
			inputSignals[0]->bufferGet(&in);
			if (count % samplesPerSymbol == 0) {

				outputSignals[0]->bufferPut((t_real)in);
			}
			myfile2 << in << "\n";
		}

	}
	return true;
}
