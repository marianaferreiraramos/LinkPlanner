#ifndef BIT_DECISION_H
#define BIT_DECISION_H

#include "netxpto_20180118.h"

class BitDecision : public Block {

public:

	/*Methods*/
	BitDecision() {};
	BitDecision(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	
	void initialize(void);
	bool runBlock(void);
};

#endif // !BIT_DECISION_H
