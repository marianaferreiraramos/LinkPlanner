# ifndef PROGRAM_FOR_HILBERT_TRANSFORM_H_
# define PROGRAM_FOR_HILBERT_TRANSFORM_H_

# include <vector>
# include "netxpto_20180118.h"

using namespace std;

class HilbertTransform : public Block
{
	/* Input Parameters */

public:
	/* Methods */
	HilbertTransform() {};
	HilbertTransform(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	void initialize(void);
	bool runBlock(void);

};
# endif


