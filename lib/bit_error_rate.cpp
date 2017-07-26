#include <algorithm>
#include <complex>

#include "netxpto.h"
#include "bit_error_rate.h"

void BitErrorRate::initialize(void){
	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool BitErrorRate::runBlock(void) {

	/* Computing z */ // This code converges in below 10 steps, exactness chosen in order to achieve this rapid convergence
	if (firstPass)
	{
		firstPass = 0;
		t_real x1 = -2;
		t_real x2 = 2;
		t_real x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
		t_real exacteness = 1e-15;

		while (abs(erf(x3 / sqrt(2)) + 1 - alpha) > exacteness)
		{
			x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
			x1 = x2;
			x2 = x3;
		}

		z = -x3;

	}


	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	ofstream translate2;
	translate2.open("translate2.txt", fstream::app);

	/* Outputting final report */

	if (process == 0)
	{
		translate2.close();

		/* Calculating BER and bounds */

		t_real NumberOfBits = receivedBits;
		t_real Coincidences = coincidences;

		t_real BER;
		BER = (NumberOfBits - Coincidences) / NumberOfBits;

		t_real UpperBound = BER + 1 / sqrt(NumberOfBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * NumberOfBits)*(2 * z * z * (1 / 2 - BER) + (2 - BER));
		t_real LowerBound = BER - 1 / sqrt(NumberOfBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * NumberOfBits)*(2 * z * z * (1 / 2 - BER) - (1 + BER));

		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open("BER.txt");
		myfile << "BER= " << BER << "\n";
		myfile << "Upper and lower confidence bounds for " << (1 - alpha)*100 << "% confidence level \n";
		myfile << "Upper Bound= " << UpperBound << "\n";
		myfile << "Lower Bound= " << LowerBound << "\n";
		myfile << "Number of received bits =" << NumberOfBits << "\n";
		myfile.close();
		return false;
	}



	for (long int i = 0; i < process; i++) {

		t_binary signalValue;
		inputSignals[0]->bufferGet(&signalValue);
		t_binary SignalValue;
		inputSignals[1]->bufferGet(&SignalValue);

		/* Outputting mid reports */
		if (m > 0)
		{
			if (remainder(receivedBits, m) == 0 && receivedBits>0)
			{
				n++;

				ostringstream oss;
				oss << "midreport" << n << ".txt";
				string filename = oss.str();

				/* Calculating BER and bounds */

				t_real NumberOfBits = receivedBits;
				t_real Coincidences = coincidences;

				t_real BER;
				BER = (NumberOfBits - Coincidences) / NumberOfBits;

				t_real UpperBound = BER + 1 / sqrt(NumberOfBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * NumberOfBits)*(2 * z * z * (1 / 2 - BER) + (2 - BER));
				t_real LowerBound = BER - 1 / sqrt(NumberOfBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * NumberOfBits)*(2 * z * z * (1 / 2 - BER) - (1 + BER));

				/* Outputting a .txt report*/
				ofstream myfile;
				myfile.open(filename);
				myfile << "BER= " << BER << "\n";
				myfile << "Upper and lower confidence bounds for" << 1-alpha << "confidence level \n";
				myfile << "Upper Bound= " << UpperBound << "\n";
				myfile << "Lower Bound= " << LowerBound << "\n";
				myfile << "Number of received bits =" << NumberOfBits << "\n";
				myfile.close();
			}
		}

		receivedBits++;

		if (signalValue == SignalValue)
		{
			coincidences++;

			outputSignals[0]->bufferPut(1);

			translate2 << 1 << "\n";
		}
		else
		{

			outputSignals[0]->bufferPut(0);

			translate2 << 0 << "\n";
		}



	}
		return true;

	}
