# include "netxpto.h"

# include "i_homodyne_receiver.h"
# include "sink.h"
# include "local_oscillator.h"
# include "balanced_beam_splitter.h"
# include "photodiode.h"
# include "ti_amplifier.h"
# include "optical_hybrid.h"


int main(){

	// #####################################################################################################
	// #################################### System Input Parameters ########################################
	// #####################################################################################################

	int numberOfBitsReceived(-1);
	int numberOfBitsGenerated(10000);
	int samplesPerSymbol(1);
	double bitPeriod = 1.0 / 5e6;
	double localOscillatorPower_dBm1 = 10;
	double localOscillatorPower2 = 6.4078e-13 * 5; // 1.0252e-11 is the power of one photon for dt=2e-7s
	double localOscillatorPhase1 = PI/4;
	double localOscillatorPhase2 = 0;
	array<t_complex, 4> transferMatrix = { { 1 / sqrt(2), 1 / sqrt(2), 1 / sqrt(2), -1 / sqrt(2)} };
	double responsivity = 1;
	double amplification = 1e6;
	double electricalNoiseAmplitude = 0; // Only quantum shot noise was used
	int bufferLength = 512*2;
	bool shotNoise(true);
	int aux = 1;

	double SNR = 0;
		
	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	OpticalSignal S1("S1.sgn");
	S1.setBufferLength(bufferLength);

	OpticalSignal S2("S2.sgn");
	S2.setBufferLength(bufferLength);


	OpticalSignal S3("S3.sgn");
	S3.setBufferLength(bufferLength);

	OpticalSignal S4("S4.sgn");
	S4.setBufferLength(bufferLength);

	OpticalSignal S5("S5.sgn");
	S5.setBufferLength(bufferLength);

	OpticalSignal S6("S6.sgn");
	S6.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S7("S7.sgn");
	S7.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S8("S8.sgn");
	S8.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S9("S9.sgn");
	S9.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S10("S10.sgn");
	S10.setBufferLength(bufferLength);

	
	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	LocalOscillator B1{ vector<Signal*> { }, vector<Signal*> { &S1 } };
	B1.setOpticalPower_dBm(localOscillatorPower_dBm1);
	B1.setPhase(localOscillatorPhase1);
	B1.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B1.setSymbolPeriod(bitPeriod);
	B1.setSignaltoNoiseRatio(SNR);

	LocalOscillator B2{ vector<Signal*> { }, vector<Signal*> { &S2 } };
	B2.setOpticalPower(localOscillatorPower2);
	B2.setPhase(localOscillatorPhase2);
	B2.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B2.setSymbolPeriod(bitPeriod);
	B2.setSignaltoNoiseRatio(SNR);

	OpticalHybrid B3{ vector<Signal*> {&S1, &S2}, vector<Signal*> {&S3, &S4, &S5, &S6} };

	Photodiode B4{ vector<Signal*> {&S3, &S4}, vector<Signal*> {&S7} };
	B4.useNoise(true);
	B4.setResponsivity(responsivity);

	TI_Amplifier B5{ vector<Signal*> {&S7}, vector<Signal*> {&S9} };
	B5.setGain(amplification);
	B5.setElectricalNoiseSpectralDensity(electricalNoiseAmplitude);
	B5.setSaveInternalSignals(true);
	B5.setSeeBeginningOfImpulseResponse(false);
	B5.setImpulseResponseLength(16);
	B5.setRollOffFactor(0);
	B5.usePassiveFilterMode(true);

	Photodiode B6{ vector<Signal*> {&S5, &S6}, vector<Signal*> {&S8} };
	B6.useNoise(true);
	B6.setResponsivity(responsivity);

	TI_Amplifier B7{ vector<Signal*> {&S8}, vector<Signal*> {&S10} };
	B7.setGain(amplification);
	B7.setElectricalNoiseSpectralDensity(electricalNoiseAmplitude);
	B7.setSaveInternalSignals(true);
	B7.setSeeBeginningOfImpulseResponse(false);
	B7.setImpulseResponseLength(16);
	B7.setRollOffFactor(0);
	B7.usePassiveFilterMode(true);


	Sink B8{ vector<Signal*> {&S9, &S10}, vector<Signal*> {} };
	B8.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B8.setDisplayNumberOfSamples(true);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8 } };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}