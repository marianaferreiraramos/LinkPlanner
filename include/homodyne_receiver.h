# ifndef PROGRAM_INCLUDE_HOMODYNE_RECEIVER_H_
# define PROGRAM_INCLUDE_HOMODYNE_RECEIVER_H_

# include "netxpto.h"

# include "optical_hybrid.h"
# include "photodiode.h"
# include "subtractor.h"
# include "ti_amplifier.h"
# include "decoder.h"
# include "local_oscillator.h"
# include "sampler.h"
# include "super_block_interface.h"
# include "pulse_shaper.h"
# include "clock.h"


// this is a test block for the purpose of beta testing new code
// current code: Building Homodyne superblock.
class HomodyneReceiver : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	//OpticalSignal HMD00{ "HMD00.sgn" }; // Input Signal

	OpticalSignal HMD01{ "HMD01.sgn" }; // Local Oscillator

	OpticalSignal HMD02{ "HMD02.sgn" }; // S+L

	OpticalSignal HMD03{ "HMD03.sgn" }; // S-L

	OpticalSignal HMD04{ "HMD04.sgn" }; // S+iL

	OpticalSignal HMD05{ "HMD05.sgn" }; // S-ilL

	TimeContinuousAmplitudeContinuousReal HMD06{ "HMD06.sgn" }; // Subtracted

	TimeContinuousAmplitudeContinuousReal HMD07{ "HMD07.sgn" }; // Substracted

	TimeContinuousAmplitudeContinuousReal HMD08{ "HMD08.sgn" }; // Amplified

	TimeContinuousAmplitudeContinuousReal HMD09{ "HMD09.sgn" }; // Amplified

	TimeContinuousAmplitudeContinuousReal HMD10{ "HMD10.sgn" }; //Filtered (pulse shaper)

	TimeContinuousAmplitudeContinuousReal HMD11{ "HMD11.sgn" }; //Filtered

	TimeContinuousAmplitudeContinuousReal HMD12{ "HMD12.sgn" }; //Clock

	TimeDiscreteAmplitudeContinuousReal HMD13{ "HMD13.sgn" }; // Sampled 

	TimeDiscreteAmplitudeContinuousReal HMD14{ "HMD14.sgn" }; // Sampled 

	Binary HMD15{ "HMD15.sgn" }; // recovery Sequence


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	LocalOscillator B1;

	OpticalHybrid B2;

	Photodiode B3;

	Photodiode B4;

	TIAmplifier B5;

	TIAmplifier B6;

	PulseShaper B7;

	PulseShaper B8;

	Clock B9;

	Sampler B10;

	Sampler B11;
	
	Decoder B12;

	/* State Variables */

	/* Input Parameters */
	double samplingPeriod{ 0.0 };

	double localOscillatorOpticalPower{ 1e-3 };
	double localOscillatorPhase{ 0 };
	double localOscillatorWavelength{ 1550e-9 };

	//int samplesToSkip{ 0 };

	double outputOpticalFrequency{ SPEED_OF_LIGHT / localOscillatorWavelength };

public:

	/* Methods */

	HomodyneReceiver(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);

	/* Set Methods */

	void setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues) { B12.setIqAmplitudes(iqAmplitudesValues); };
	vector<t_iqValues> const getIqAmplitudes(void) { return B12.getIqAmplitudes(); };

	void setLocalOscillatorSamplingPeriod(double sPeriod) { B1.setSamplingPeriod(sPeriod); };
	void setLocalOscillatorOpticalPower(double opticalPower) { B1.setOpticalPower(opticalPower); };
	void setLocalOscillatorOpticalPower_dBm(double opticalPower_dBm) { B1.setOpticalPower_dBm(opticalPower_dBm); };
	void setLocalOscillatorPhase(double lOscillatorPhase) { B1.setPhase(lOscillatorPhase); };
	void setLocalOscillatorOpticalWavelength(double lOscillatorWavelength) { B1.setWavelength(lOscillatorWavelength); };

	void setSamplingPeriod(double sPeriod) { B1.setSamplingPeriod(sPeriod); B9.setSamplingPeriod(sPeriod); };

	void  setResponsivity(t_real Responsivity) { B3.responsivity = Responsivity; B4.responsivity = Responsivity; };

	void setAmplification(t_real Amplification) { B5.amplification = Amplification; B6.amplification = Amplification; };
	void setNoiseAmplitude(t_real NoiseAmplitude) { B5.noiseamp = NoiseAmplitude; B6.noiseamp = NoiseAmplitude;};

	void setImpulseResponseTimeLength(int impResponseTimeLength) { B7.setImpulseResponseTimeLength(impResponseTimeLength); B8.setImpulseResponseTimeLength(impResponseTimeLength); };
	void setFilterType(PulseShaperFilter fType) { B7.setFilterType(fType); B8.setFilterType(fType); };
	void setRollOffFactor(double rOffFactor) { B7.setRollOffFactor(rOffFactor); B8.setRollOffFactor(rOffFactor); };

	void setClockPeriod(double per) { B9.setClockPeriod(per); };

	void setSamplesToSkip(int sToSkip) { B10.setSamplesToSkip(sToSkip); B11.setSamplesToSkip(sToSkip); };

	void setSamplerOpticalPower_dBm(double optPower_dBm) { B10.setOutputOpticalPower_dBm(optPower_dBm); };

};

#endif
