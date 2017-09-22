# include "netxpto.h"
# include "pulse_shaper_freq.h"

using namespace std;

void raisedCosineTF(vector<t_complex> &transferFunction, int transferFunctionLength, double rollOffFactor, double samplingPeriod, double symbolPeriod);

void FD_PulseShaper::initialize(void) {

	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;


	transferFunction.resize(getTransferFunctionLength());
	
	switch (getFilterType()) {

		case RaisedCosine:
			raisedCosineTF(transferFunction, getTransferFunctionLength(), rollOffFactor, samplingPeriod, symbolPeriod);
			break;
	};

	initializeFD_Filter();

}


void raisedCosineTF(vector<t_complex> &transferFunction, int transferFunctionLength, double rollOffFactor, double samplingPeriod, double symbolPeriod) {

	double cond1_if = (1 - rollOffFactor) / (2 * symbolPeriod);
	double cond2_if = (1 + rollOffFactor) / (2 * symbolPeriod);

	t_complex transferFunction_normFactor(0);
	vector<t_real> freqHz(transferFunctionLength, (t_real) 0.0);

	for (int i = 0; i < transferFunctionLength; i++) {
		freqHz.at(i) = -(1 / (samplingPeriod * 2)) + i*(1 / (samplingPeriod * transferFunctionLength));
		if (abs(freqHz.at(i)) <= (cond1_if)) {
			transferFunction[i] = 1.0;
		}
		else if ((abs(freqHz.at(i))>cond1_if) && (abs(freqHz.at(i)) <= cond2_if)){
			transferFunction[i] = (1.0 / 2.0)*(1.0 + cos((PI*symbolPeriod / rollOffFactor)*(abs(freqHz.at(i)) - ((1.0 - rollOffFactor) / (2.0 * symbolPeriod)))));

		}
		else {
			transferFunction[i] = 0;
		}
		//transferFunction_normFactor +=  (transferFunction[i] * transferFunction[i]);
	};
	
	// Normalization to unit energy
	/*for (int i = 0; i < transferFunctionLength; i++) {
		transferFunction[i] = transferFunction[i] / sqrt(transferFunction_normFactor);
	}*/
	rotate(transferFunction.begin(), transferFunction.begin() + transferFunctionLength / 2, transferFunction.end());

};
