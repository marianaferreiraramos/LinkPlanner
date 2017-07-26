# include "netxpto.h"

# include "m_qam_transmitter.h"
# include "i_homodyne_receiver.h"
# include "bit_error_rate.h"
# include "local_oscillator.h"
# include "balanced_beam_splitter.h"
# include "photodiode.h"
# include "ti_amplifier.h"
# include "sampler.h"
# include "sink.h"

int main(){

	// #####################################################################################################
	// #################################### System Input Parameters ########################################
	// #####################################################################################################

	int numberOfBitsReceived(-1);
	int numberOfBitsGenerated(1000);
	int samplesPerSymbol(16);
	int pLength = 5;
	double bitPeriod = 1.0 / 5e6;
	double rollOffFactor = 0.3;
	vector<t_iqValues> iqAmplitudeValues = { { -1, 0 }, { 1, 0 } };
	double signalOutputPower_dBm = -4; // doesn't matter right now
	double localOscillatorPower_dBm1 = -20;
	double localOscillatorPower2 = 0;
	double localOscillatorPhase1 = 0;
	double localOscillatorPhase2 = PI;
	array<t_complex, 4> transferMatrix = { { 1 / sqrt(2), 1 / sqrt(2), 1 / sqrt(2), -1 / sqrt(2)} };
	double responsivity = 1;
	double amplification = 1e6;
	double electricalNoiseAmplitude = 0.0022*0.0022;
	int samplesToSkip = 16*16;// 8 * samplesPerSymbol;
	int bufferLength = 512;
	bool shotNoise(true);
		
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

	TimeContinuousAmplitudeContinuousReal S5("S5.sgn");
	S5.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S6("S6.sgn");
	S6.setBufferLength(bufferLength);

	TimeDiscreteAmplitudeContinuousReal S7("S7.sgn");
	S7.setBufferLength(bufferLength);


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

<<<<<<< HEAD
	LocalOscillator B1{ vector<Signal*> { }, vector<Signal*> { &S1 } };
	B1.setOpticalPower_dBm(localOscillatorPower_dBm1);
	B1.setPhase(localOscillatorPhase1);
	B1.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B1.setSymbolPeriod(bitPeriod);

	LocalOscillator B2{ vector<Signal*> { }, vector<Signal*> { &S2 } };
	B2.setOpticalPower(localOscillatorPower2);
	B2.setPhase(localOscillatorPhase2);
	B2.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B2.setSymbolPeriod(bitPeriod);

	BalancedBeamSplitter B3{ vector<Signal*> {&S1, &S2}, vector<Signal*> {&S3, &S4} };
	B3.setTransferMatrix(transferMatrix);

	Photodiode B4{ vector<Signal*> {&S3, &S4}, vector<Signal*> {&S5} };
	B4.useNoise(shotNoise);
	B4.setResponsivity(responsivity);

	TI_Amplifier B5{ vector<Signal*> {&S5}, vector<Signal*> {&S6} };
	B5.setGain(amplification);
	B5.setElectricalNoiseSpectralDensity(electricalNoiseAmplitude);
	B5.setSaveInternalSignals(true);
	B5.setSeeBeginningOfImpulseResponse(false);
	B5.setImpulseResponseLength(16);
	B5.setRollOffFactor(0);
	B5.usePassiveFilterMode(true);

	Sampler B6{ vector<Signal*> {&S6}, vector<Signal*> {&S7} };

	Sink B7{ vector<Signal*> { &S7 }, vector<Signal*> {} };
	B7.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B7.setDisplayNumberOfSamples(true);
=======
	MQamTransmitter B1{ vector<Signal*> { }, vector<Signal*> { &S1, &S0 } };
	B1.setNumberOfBits(numberOfBitsGenerated);
	B1.setOutputOpticalPower_dBm(signalOutputPower_dBm);
	B1.setMode(DeterministicAppendZeros);
	B1.setBitStream("010");
	B1.setBitPeriod(bitPeriod);
	B1.setPatternLength(pLength);
	B1.setIqAmplitudes(iqAmplitudeValues);
	B1.setNumberOfSamplesPerSymbol(samplesPerSymbol);
	B1.setRollOffFactor(rollOffFactor);
	B1.setSaveInternalSignals(true);
	B1.setSeeBeginningOfImpulseResponse(true);

	I_HomodyneReceiver B2{ vector<Signal*> {&S1}, vector<Signal*> {&S2} };
	B2.setLocalOscillatorOpticalPower_dBm(localOscillatorPower_dBm);
	B2.setLocalOscillatorPhase(localOscillatorPhase);
	B2.setLocalOscillatorSamplingPeriod(bitPeriod / samplesPerSymbol);
	//B2.setLocalOscillatorSymbolPeriod(bitPeriod);
	B2.setTransferMatrix(transferMatrix);
	B2.setResponsivity(responsivity);
	B2.setAmplification(amplification);
	B2.setNoiseAmplitude(noiseAmplitude);
	B2.setSamplesToSkip(samplesToSkip);
	B2.setSaveInternalSignals(true);

	Sink B3{ vector<Signal*> { &S0 }, vector<Signal*> {} };
	B3.setNumberOfSamples(numberOfBitsReceived*samplesPerSymbol);
	B3.setDisplayNumberOfSamples(true);

	Sink B4{ vector<Signal*> { &S2 }, vector<Signal*> {} };
	B4.setNumberOfSamples(numberOfBitsReceived*samplesPerSymbol);
	B4.setDisplayNumberOfSamples(true);
>>>>>>> AnaLuisa


	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B7 } };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}