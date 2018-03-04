#ifndef COINCIDENCE_DETECTOR_H
#define COINCIDENCE_DETECTOR_H

#include "netxpto_20180118.h"

class CoincidenceDetector : public Block {
	/*Input Parameters*/
	bool firstTime{ true };
public:

	CoincidenceDetector() {};
	CoincidenceDetector(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);
};

#endif
