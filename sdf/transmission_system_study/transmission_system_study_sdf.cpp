// Transmissão de um sinal binário usando apenas 1 base.

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
# include "sampler.h"
# include "bit_decider.h"


int main()
{

	// #########################################################################
	// ######################## System Input Parameters ########################
	// #########################################################################

	int numberOfBitsGenerated(500);

	int samplesPerSymbol(16);
	double bitPeriod = 1.0E-9;

	// powerUnit = PLANCK_CONSTANT * SPEED_OF_LIGHT / (bitPeriod*wavelength)
	// We will assume: wavelength = 1.550E-6
	//double powerUnit = 6.4078e-13;
	double wavelength = 1.55e-6;
	double samplePeriod = bitPeriod / samplesPerSymbol;  // Talvez isto não seja preciso
	
	
	double powerUnit = PLANCK_CONSTANT*SPEED_OF_LIGHT / (samplePeriod*wavelength);
	// Correcção da potência.
	//double powerUnit = PLANCK_CONSTANT*SPEED_OF_LIGHT / (bitPeriod*wavelength);

	double localOscillatorPower1 = powerUnit * 1;
	double localOscillatorPower2 = powerUnit * 1e4;

	double localOscillatorPhase = 0;
	array<t_complex, 4> transferMatrix = { { 1 / sqrt(2), 1 / sqrt(2), 1 / sqrt(2), -1 / sqrt(2) } };
	double responsivity = 1;
	
	//double amplification = 1e6;
	//double electricalNoiseAmplitude = 0;// 0.0022*0.0022;
	
	int bufferLength = 512;
	bool shotNoise(true);

	// Escolha da constelação
	// Constelação sempre com o mesmo estado
	vector<t_iqValues> iqAmplitudeValues = { { 1, 1 }, { 1, 1 } };
	// Constelação com 2 estados
	//vector<t_iqValues> iqAmplitudeValues = { { -1, 0 },{ 1, 0 } };
	// Constelação com 4 estados
	//vector<t_iqValues> iqAmplitudeValues = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

	double SNR = 0;

	// Number of samples to avoid the delay created in the MQAM
	int samplesToSkip = samplesPerSymbol * 8;




	// #########################################################################
	// ################ Signals Declaration and Inicialization #################
	// #########################################################################




	// %%%%%%%%%%%%%%%% ALICE SIGNAL GENERATION %%%%%%%%%%%%%%%%

	// Geração dos sinais binários
	Binary S0("S0.sgn");
	S0.setBufferLength(bufferLength);

	// Sinal óptico 
	OpticalSignal S1("S1.sgn");
	S1.setBufferLength(bufferLength);




	// %%%%%%%%%%%%%%%%   BOB SIGNAL DETECTION   %%%%%%%%%%%%%%%%

	OpticalSignal S2("S2.sgn");
	S2.setBufferLength(bufferLength);

	OpticalSignal S3("S3.sgn");
	S3.setBufferLength(bufferLength);

	OpticalSignal S4("S4.sgn");
	S4.setBufferLength(bufferLength);

	OpticalSignal S5("S5.sgn");
	S3.setBufferLength(bufferLength);

	OpticalSignal S6("S6.sgn");
	S6.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S7("S7.sgn");
	S7.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal S8("S8.sgn");
	S8.setBufferLength(bufferLength);

	TimeDiscreteAmplitudeContinuousReal S9("S9.sgn");
	S9.setBufferLength(bufferLength);

	TimeDiscreteAmplitudeContinuousReal S10("S10.sgn");
	S10.setBufferLength(bufferLength);

	OpticalSignal S11("S11.sgn");
	S11.setBufferLength(bufferLength);




	// #########################################################################
	// ################# Blocks Declaration and Inicialization #################
	// #########################################################################

	// BEGIN ALICE

	MQamTransmitter B1{ vector<Signal*> {}, vector<Signal*> {&S1, &S0} };
	B1.setNumberOfBits(numberOfBitsGenerated);
	B1.setOutputOpticalPower(localOscillatorPower1);

	B1.setMode(PseudoRandom);

	B1.setBitPeriod(bitPeriod);
	B1.setIqAmplitudes(iqAmplitudeValues);
	B1.setPulseShaperFilter(Gaussian);

	B1.setNumberOfSamplesPerSymbol(samplesPerSymbol);
	B1.setSaveInternalSignals(true);
	B1.setSeeBeginningOfImpulseResponse(true);

	// END ALICE




	// BEGIN BOB.

	LocalOscillator B2{ vector<Signal*> {}, vector<Signal*> {&S2} };
	B2.setOpticalPower(localOscillatorPower2);
	B2.setPhase(localOscillatorPhase);
	B2.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B2.setSymbolPeriod(bitPeriod);
	B2.setSignaltoNoiseRatio(SNR);

	OpticalHybrid B3{ vector<Signal*> {&S1, &S2},
					   vector<Signal*> {&S3, &S4, &S5, &S6} };

	Photodiode B4{ vector<Signal*> { &S3, &S4 }, vector<Signal*> {&S7} };
	B4.useNoise(shotNoise);
	B4.setResponsivity(responsivity);

	Photodiode B5{ vector<Signal*> { &S5, &S6 }, vector<Signal*> {&S8} };
	B5.useNoise(shotNoise);
	B5.setResponsivity(responsivity);

	Sampler B6{ vector<Signal*> { &S7 }, vector<Signal*> { &S9 } };
	B6.setSamplesToSkip(samplesToSkip);

	Sampler B7{ vector<Signal*> { &S8 }, vector<Signal*> { &S10 } };
	B7.setSamplesToSkip(samplesToSkip);

	// Conjugação dos dois sinais vindos dos samplers para criar um sinal complexo
	// de modo a poder ver-se a constelação.
	IqModulator B8{ vector<Signal*> { &S9, &S10 }, vector<Signal*> { &S11 } };

	// END BOB



	/*
	Sink B8{ vector<Signal*> {&S9}, vector<Signal*> {} };
	B8.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B8.setDisplayNumberOfSamples(true);

	*/

	Sink B9{ vector<Signal*> {&S11}, vector<Signal*> {} };
	B9.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B9.setDisplayNumberOfSamples(true);



	Sink B10{ vector<Signal*> {&S0}, vector<Signal*> {} };
	B10.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B10.setDisplayNumberOfSamples(true);




	// #########################################################################
	// ################# System Declaration and Inicialization #################
	// #########################################################################

	System MainSystem{ vector<Block*> {&B1, &B2, &B3, &B4, &B5, &B6, &B7,
		&B8, &B9, &B10} };




	// #########################################################################
	// ############################## System Run ###############################
	// #########################################################################

	MainSystem.run();

	return 0;
}