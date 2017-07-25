# include <algorithm> // min
# include <iostream> //write to file
# include <fstream>

# include "netxpto.h"
# include "discrete_to_continuous_time.h"

void DiscreteToContinuousTime::initialize(void) {

	outputSignals[0]->symbolPeriod = (inputSignals[0]->symbolPeriod);
	outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol;
	outputSignals[0]->samplesPerSymbol = numberOfSamplesPerSymbol;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	myfile1.open("debug1.txt");
}

bool DiscreteToContinuousTime::runBlock(void) {

	myfile1 << "count1=" << count1 << endl;

	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	
	if (index!=0) {
		int length = min(space, numberOfSamplesPerSymbol - index);
		if (length > 0) {
			alive = true;
			for (int i = 0; i < length; i++) {
				outputSignals[0]->bufferPut(0);
				index++;
			};
		}
		index = index % numberOfSamplesPerSymbol;
	};

	int length = min((int)(floor(space / numberOfSamplesPerSymbol)), ready);

	if (length <= 0) return alive;

	signal_value_type inSignalType = inputSignals[0]->getValueType();
	switch (inSignalType) {
		case RealValue:
			for (int i = 0; i < length; i++) {

				t_real value;
				(inputSignals[0])->bufferGet(&value);
				outputSignals[0]->bufferPut((t_real) value);
						
				space--;
				index++;

				for (int k = 1; (k<numberOfSamplesPerSymbol) & (space>0); k++) {
					outputSignals[0]->bufferPut((t_real) 0.0);

					space--;
					index++;
				}
				if (index == numberOfSamplesPerSymbol) index = 0;

			}
			return true;
		case BinaryValue:
			for (int i = 0; i < length; i++) {
				t_binary value;
				(inputSignals[0])->bufferGet(&value);
				if (value == 0) {
					outputSignals[0]->bufferPut((t_real) 0.0);
				}
				else {
					outputSignals[0]->bufferPut((t_real) 1.0);
				}
				space--;
				index++;
				for (int k = 1; (k<numberOfSamplesPerSymbol) & (space>0); k++) {
					outputSignals[0]->bufferPut((t_real) 0.0);
					space--;
					index++;
				}
				index = index % numberOfSamplesPerSymbol;

				//if (space<=0) break;
			}
			return true;
		default:
			cout << "ERRO: discrete_to_continuous_time.cpp" << "\n";
			return false;
	};

	count1++;

};
