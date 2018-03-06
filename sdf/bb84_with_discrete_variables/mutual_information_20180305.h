#ifndef MUTUAL_INFORMATION_H
#define MUTUAL_INFORMATION_H

#include "netxpto_20180118.h"

class MutualInformation : public Block {
	/*Input Parameters*/

	/*State Variables*/

public:
	/*Methods*/
	MutualInformation() {};
	MutualInformation(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

};

#endif // !MUTUAL_INFORMATION_H

