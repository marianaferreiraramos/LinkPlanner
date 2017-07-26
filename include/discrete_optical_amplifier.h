# ifndef PROGRAM_DISCRETE_OPTICAL_AMPLIFIER_H_
# define PROGRAM_DISCRETE_OPTICAL_AMPLIFIER_H_

# include "netxpto.h"

# include "ideal_amplifier.h"
# include "add.h"
# include "white_noise.h"


// Implements a Discrete Optical Amplifier
class DiscreteOpticalAmplifier : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	OpticalSignal EDFA01{ "EDFA01.sgn" }; // Amplified

	OpticalSignal EDFA02{ "EDFA02.sgn" }; // White Noise

	OpticalSignal EDFA03{ "EDFA03.sgn" }; // Out Signal

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	IdealAmplifier B1;

	WhiteNoise B2;

	Add B3;

	/* State Variables */

	/* Input Parameters */
	double gain{ 1e4 };

	double bilateralNoiseSpectralDensity{ 1e-3 }; // W/Hz
	

public:

	/* Methods */

	DiscreteOpticalAmplifier(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);

	/* Set Methods */

	void setGain(double gain) { B1.setGain(gain); };

	void setOpticalNoiseSpectralDensity(double std) { B2.setNoiseSpectralDensity(std); };
	double const getOpticalNoiseSpectralDensity(void) { B2.getNoiseSpectralDensity(); }

};

#endif
