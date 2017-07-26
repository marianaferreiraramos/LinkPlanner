# include <algorithm> // min

# include "netxpto.h"
# include "discrete_to_continuous_time.h"

void DiscreteToContinuousTime::initialize(void) {

	outputSignals[0]->symbolPeriod = (inputSignals[0]->symbolPeriod);
	outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol;
	outputSignals[0]->samplesPerSymbol = numberOfSamplesPerSymbol;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool DiscreteToContinuousTime::runBlock(void) {

	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	
	int length = min(space, ready+index);

	if (length <= 0) return alive;

	signal_value_type inSignalType = inputSignals[0]->getValueType();
	switch (inSignalType) {
		case RealValue:
			for (int i = 0; i < length; i++) {
				t_real value;
				if (index == 0) {
					(inputSignals[0])->bufferGet(&value);
					outputSignals[0]->bufferPut(value);
				}
				else {
					outputSignals[0]->bufferPut((t_real) 0.0);
				}
				index++;
				index = index % numberOfSamplesPerSymbol;
			}
			return true;
		case BinaryValue:
			for (int i = 0; i < length; i++) {
				t_binary value;
				if (index == 0) {
					(inputSignals[0])->bufferGet(&value);
					if (value == 0) {
						outputSignals[0]->bufferPut((t_real) 0.0);
					}
					else {
						outputSignals[0]->bufferPut((t_real) 1.0);
					}
				}
				else {
					outputSignals[0]->bufferPut((t_real) 0.0);
				}
				index++;
				index = index % numberOfSamplesPerSymbol;
			}
			return true;
		default:
			cout << "ERRO: discrete_to_continuous_time.cpp" << "\n";
			return false;
	};

};
