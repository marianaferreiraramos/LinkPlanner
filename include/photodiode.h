# ifndef PROGRAM_INCLUDE_PHOTODIODE_H_
# define PROGRAM_INCLUDE_PHOTODIODE_H_

# include "netxpto.h"
#include <random>

// Simulates a photodiode
class Photodiode : public Block {

	bool firstTime{ true };
	int aux = 0;
	bool firstPass{ true };
	double t = 0;

public:

	double frequencyMismatch{ 1.9441e+11 };
	double responsivity = 1;
	bool shotNoise = false;

	default_random_engine generatorAmp1;
	default_random_engine generatorAmp2;

	Photodiode() {};
	Photodiode(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setResponsivity(double Responsivity) { responsivity = Responsivity; }
	double const getResponsivity(void) { return responsivity; }

	void useNoise(bool sNoise) { shotNoise = sNoise; }

private:
};


#endif // !PROGRAM_INCLUDE_PHOTODIODE_H_
