# include "netxpto.h"

# include "m_qam_transmitter.h"
# include "i_homodyne_receiver.h"
# include "sink.h"
# include "bit_error_rate.h"
# include "local_oscillator.h"
# include "balanced_beam_splitter.h"
# include "photodiode.h"
# include "ti_amplifier.h"
# include "sampler.h"
# include "optical_hybrid.h"
# include "testblock.h"

int main(){

	// #####################################################################################################
	// #################################### System Input Parameters ########################################
	// #####################################################################################################

	int numberOfBitsGenerated(100);
	int samplesPerSymbol(160);
	double bitPeriod = .5e-9;
	double localOscillatorPower_dBm = 10;
	double signalPower = 6.4078e-13 * 5; // 1.0252e-11 is the power of one photon for dt=2e-7s
	double localOscillatorPhase = 0;
	array<t_complex, 4> transferMatrix = { { 1 / sqrt(2), 1 / sqrt(2), 1 / sqrt(2), -1 / sqrt(2)} };
	double responsivity = 1;
	double amplification = 1e6;
	double electricalNoiseAmplitude = 0;// 0.0022*0.0022;
	int bufferLength = 512;
	bool shotNoise(false);
	int aux = 1;
	vector<t_iqValues> iqAmplitudeValues = { { -1, 0 }, { 1, 0 } };

	double SNR = 0;
		
	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	OpticalSignal S1("S1.sgn");
	S1.setBufferLength(bufferLength);
	S1.setFirstValueToBeSaved(0);

	OpticalSignal S2("S2.sgn");
	S2.setBufferLength(bufferLength);
	S2.setFirstValueToBeSaved(0);

	OpticalSignal S3("S3.sgn");
	S3.setBufferLength(bufferLength);

	OpticalSignal S4("S4.sgn");
	S4.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S5("S5.sgn");
	S5.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S6("S6.sgn");
	S6.setBufferLength(bufferLength);

	
	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	/*LocalOscillator B1{ vector<Signal*> { }, vector<Signal*> { &S1 } };
	B1.setOpticalPower_dBm(localOscillatorPower_dBm1);
	B1.setPhase(localOscillatorPhase1);
	B1.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B1.setSymbolPeriod(bitPeriod);
	B1.setSignaltoNoiseRatio(SNR);*/

	MQamTransmitter B1{ vector<Signal*> {}, vector<Signal*> {&S1} };
	B1.setNumberOfBits(numberOfBitsGenerated);
	B1.setOutputOpticalPower(signalPower);
	B1.setMode(PseudoRandom);
	B1.setBitPeriod(bitPeriod);
	B1.setIqAmplitudes(iqAmplitudeValues);
	B1.setPulseShaperFilter(Gaussian);
	B1.setNumberOfSamplesPerSymbol(samplesPerSymbol);
	B1.setSaveInternalSignals(true);
	B1.setSeeBeginningOfImpulseResponse(false);

	LocalOscillator B2{ vector<Signal*> { }, vector<Signal*> { &S2 } };
	B2.setOpticalPower(localOscillatorPower_dBm);
	B2.setPhase(localOscillatorPhase);
	B2.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B2.setSymbolPeriod(bitPeriod);
	B2.setSignaltoNoiseRatio(SNR);

	BalancedBeamSplitter B3{ vector<Signal*> {&S1, &S2}, vector<Signal*> {&S3, &S4 } };
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


	Sink B6{ vector<Signal*> {&S6}, vector<Signal*> {} };
	B6.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B6.setDisplayNumberOfSamples(true);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6 } };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}