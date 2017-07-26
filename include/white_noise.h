# ifndef PROGRAM_INCLUDE_WHITE_NOISE_H_
# define PROGRAM_INCLUDE_WHITE_NOISE_H_

# include "netxpto.h"
# include <vector>
#include <random>

// Simulates a Transimpedance Amplifier
class WhiteNoise : public Block {

	bool firsTime{ true };

public:

	double spectralDensity = 1e-4;
	
	default_random_engine generator1;
	default_random_engine generator2;
	default_random_engine generator3;
	default_random_engine generator4;


	WhiteNoise() {};
	WhiteNoise(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setNoiseSpectralDensity(double SpectralDensity) { spectralDensity = SpectralDensity; }
	double const getNoiseSpectralDensity(void){ return spectralDensity; }

private:
};


#endif // !PROGRAM_INCLUDE_TIAMPLIFIER_H_
