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
	
	// [DIA] Original
	//int numberOfBitsGenerated(10000);
	// [DIA] Experiencia
	int numberOfBitsGenerated(500);

	int samplesPerSymbol(1);
	double bitPeriod = 1.0 / 5e6;
	
	// [DIA] Original
	//double localOscillatorPower_dBm1 = -20;
	// [DIA] Experiencia (Potencia de 1 fotão por sinal)
	double localOscillatorPower_dBm1 = -91.933;
	//double localOscillatorPower_dBm1 = -20;
	
	double localOscillatorPower2 = 0; // Vacuum state
	double localOscillatorPhase1 = 0;
	double localOscillatorPhase2 = 0;
	array<t_complex, 4> transferMatrix = { { 1 / sqrt(2), 1 / sqrt(2), 1 / sqrt(2), -1 / sqrt(2)} };
	double responsivity = 1;
	double amplification = 1e6;
	double electricalNoiseAmplitude = 4.7626e-06; // Thermal noise taken from experimental results. 
	int bufferLength = 512*2;
	bool shotNoise(true);

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

	TimeContinuousAmplitudeContinuousReal S5("S5.sgn");
	S5.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S6("S6.sgn");
	S6.setBufferLength(bufferLength);
	
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

	BalancedBeamSplitter B3{ vector<Signal*> {&S1, &S2}, vector<Signal*> {&S3, &S4 } };
	B3.setTransferMatrix(transferMatrix);

	Photodiode B4{ vector<Signal*> {&S3, &S4}, vector<Signal*> {&S5} };
	B4.useNoise(true);
	B4.setResponsivity(responsivity);

	// [DIA] debug
	Sink Bx{ vector<Signal*> {&S5}, vector<Signal*> {} };
	Bx.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	Bx.setDisplayNumberOfSamples(true);
	// end debug

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

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &Bx } };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}