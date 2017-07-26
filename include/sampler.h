# ifndef PROGRAM_INCLUDE_SAMPLER_H_
# define PROGRAM_INCLUDE_SAMPLER_H_

# include "netxpto.h"
# include <vector>

// Takes a sampling of the input electrical signal
class Sampler : public Block {

	bool firstTime{ true };

	bool firstPass{ true };
	bool repeatedPass{ false };
	int samplesToSkip{ 0 };
	int count = 0;
	bool aux1{ false };

public:

	Sampler() {};
	Sampler(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

	void setSamplesToSkip(int sToSkip) { samplesToSkip = sToSkip; }
	int const getSamplesToSkip(void) { return samplesToSkip; }

};


#endif // !PROGRAM_INCLUDE_SAMPLER_H_
