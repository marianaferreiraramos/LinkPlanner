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

//DIA> My includes
//include "white_noise.h"

int main() {

	// #####################################################################################################
	// #################################### System Input Parameters ########################################
	// #####################################################################################################

	// [DIA] Valor original
	//int numberOfBitsGenerated(100);
	// [DIA] Valor de teste
	int numberOfBitsGenerated(40);


	int samplesPerSymbol(160);
	double bitPeriod = .5e-9;
	double localOscillatorPower_dBm = 10;
	double signalPower = 6.4078e-13 * 5; // 1.0252e-11 is the power of one photon for dt=2e-7s
	double localOscillatorPhase = 0;
	array<t_complex, 4> transferMatrix = { { 1 / sqrt(2), 1 / sqrt(2), 1 / sqrt(2), -1 / sqrt(2) } };
	double responsivity = 1;
	double amplification = 1e6;
	double electricalNoiseAmplitude = 0;// 0.0022*0.0022;
	int bufferLength = 512;
	bool shotNoise(true);
	vector<t_iqValues> iqAmplitudeValues = { { -1, 0 },{ 1, 0 } };

	double SNR = 0;

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	// %%%%%%%%%%%%%%%% ALICE SIGNAL GENERATION %%%%%%%%%%%%%%%%

	// Geração dos sinais binários
	Binary S0("S0.sgn");
	S0.setBufferLength(bufferLength);

	// Sinal óptico 
	OpticalSignal S1("S1.sgn");
	S1.setBufferLength(bufferLength);


/*	
	// %%%%%%%%%%%%%%%%   EVE SIGNAL DETECTION   %%%%%%%%%%%%%%%%

	OpticalSignal ED0("ED0.sgn");
	ED0.setBufferLength(bufferLength);

	OpticalSignal ED1("ED1.sgn");
	ED1.setBufferLength(bufferLength);

	OpticalSignal ED2("ED2.sgn");
	ED2.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal ED3("ED3.sgn");
	ED3.setBufferLength(bufferLength);


	TimeDiscreteAmplitudeContinuousReal ED4("ED4.sgn");
	ED4.setBufferLength(bufferLength);


	Binary ED5("ED5.sgn");
	ED5.setBufferLength(bufferLength);



	// %%%%%%%%%%%%%%%% EVE SIGNAL REGENERATION %%%%%%%%%%%%%%%%

	TimeDiscreteAmplitudeDiscreteReal EE1("EE1.sgn");
	EE1.setBufferLength(bufferLength);

	TimeDiscreteAmplitudeDiscreteReal EE2("EE2.sgn");
	EE2.setBufferLength(bufferLength);

	
	TimeContinuousAmplitudeDiscreteReal EE3("EE3.sgn");
	EE3.setBufferLength(bufferLength);

	TimeContinuousAmplitudeDiscreteReal EE4("EE4.sgn");
	EE4.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal EE5("EE5.sgn");
	EE5.setBufferLength(bufferLength);

	TimeContinuousAmplitudeContinuousReal EE6("EE6.sgn");
	EE6.setBufferLength(bufferLength);

	OpticalSignal EE7("EE7.sgn");
	EE7.setBufferLength(bufferLength);

*/

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
	S4.setBufferLength(bufferLength);


	TimeContinuousAmplitudeContinuousReal S7("S7.sgn");
	S5.setBufferLength(bufferLength);

	TimeDiscreteAmplitudeContinuousReal S8("S8.sgn");
	S6.setBufferLength(bufferLength);

	Binary S7("S7.sgn");
	S7.setBufferLength(bufferLength);

	Binary S8("S8.sgn");
	S8.setBufferLength(bufferLength);



	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	// BEGIN ALICE

	// (1)
	// [DIA]
	// Troquei S1 por S0 (optico <-> binario)
	MQamTransmitter B1{ vector<Signal*> {}, vector<Signal*> {&S1, &S0} }; // CORRECCAO (qual? ver código do daniel)
	B1.setNumberOfBits(numberOfBitsGenerated);
	B1.setOutputOpticalPower(signalPower);
	B1.setMode(PseudoRandom);
	B1.setBitPeriod(bitPeriod);
	B1.setIqAmplitudes(iqAmplitudeValues);

	B1.setPulseShaperFilter(Gaussian);
	// BUG? O sinal dos sincs é desviado no tempo e mantém o comprimento do vector que sai do discrete do
	// to continuos time, o que vai amputar as samples finais do vector.

	B1.setNumberOfSamplesPerSymbol(samplesPerSymbol);
	B1.setSaveInternalSignals(true);
	B1.setSeeBeginningOfImpulseResponse(true);


	//DIA> Introdução de ruido?
	// O Ruido quântico tem que ter var = 1/4
	//WhiteNoise Bx{ vector<Signal*>{&Sx}, vector<Signal*> {&S1} };


/*

	//////// BEGIN EVE ////////

	// Estrutura base dos módulos/funções: fname {input, output}

	// SINAL DA EVE
	LocalOscillator B2{ vector<Signal*> { }, vector<Signal*> { &ED0 } };
	B2.setOpticalPower(localOscillatorPower_dBm);
	B2.setPhase(localOscillatorPhase);
	B2.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B2.setSymbolPeriod(bitPeriod);
	B2.setSignaltoNoiseRatio(SNR);

	// BEAM SPLITTER
	BalancedBeamSplitter B3{ vector<Signal*> {&S1, &ED0}, vector<Signal*> {&ED1, &ED2 } };
	B3.setTransferMatrix(transferMatrix);

	
	// LEITURA DAS POTÊNCIA (INSTANTÂNEO)
	Photodiode B4{ vector<Signal*> {&ED1, &ED2}, vector<Signal*> {&ED3} };
	B4.useNoise(shotNoise); // IMPORTANTE PARA DAR MAIS REALISMO.
	B4.setResponsivity(responsivity);

	 
	// O RUIDO VAI INDUZIR ERROS NA LEITURA
	// Escolha de apenas 1 ponto por símbolo.
	Sampler B5{ vector<Signal*> {&ED3}, vector<Signal*> {&ED4} };
	//B5.setSamplesToSkip();
	//impulseResponseLeght*SamplesPerSymbol
		


	// Ver se é 0 ou 1 conforme a voltagem ou outra cena.
	// [DIA]
	// No documento diz que existem 2 parâmetros para configurar :
	// setPosReferenceValue e setNegReferenceValue. No entanto na definição do
	// header só existe um parâmetro decisionLevel.

	BitDecider B6{ vector<Signal*> {&ED4}, vector<Signal*> {&ED5} };
	

	
	// BEGIN [Reconstrução do sinal]
	// Para "Simplificar" o processo, poderia-se injectar o sinal binário dentro do M_QAM_TRANSMITTER.
	// Este processo é parecido com o da ALICE. (na codificação)

	// TASK: "Copiar" as configurações em (1) (ex: não copiar o numero de bits)
	MQamMapper B7{ vector<Signal*> {&ED5}, vector<Signal*> {&EE1, &EE2} };
	
	DiscreteToContinuousTime  B8{ vector<Signal*> {&EE1}, vector<Signal*> {&EE3} };

	DiscreteToContinuousTime  B9{ vector<Signal*> {&EE2}, vector<Signal*> {&EE4} };

	PulseShaper B10{ vector<Signal*> {&EE3}, vector<Signal*> {&EE5} };

	PulseShaper B11{ vector<Signal*> {&EE4}, vector<Signal*> {&EE6} };

	IqModulator B12{ vector<Signal*> {&EE5, &EE6}, vector<Signal*> {&EE7} };
	
	// END [Reconstrução do sinal]


	// END EVE

*/


	// BEGIN BOB.
	// Mesmo processo da EVE.
	LocalOscillator B13{ vector<Signal*> {}, vector<Signal*> {&S2} };
	B13.setOpticalPower(localOscillatorPower_dBm);
	B13.setPhase(localOscillatorPhase);
	B13.setSamplingPeriod(bitPeriod / samplesPerSymbol);
	B13.setSymbolPeriod(bitPeriod);
	B13.setSignaltoNoiseRatio(SNR);



	//DIA> A detecção vai consistir num optical hybrid + 2 photodiodes

	OpticalHybrid B14{ vector<Signal*> {&S1, &S2}, vector<Signal*> {&S3, &S4, &S5, &S6} };

	Photodiode B15{ vector<Signal*> { &S3, &S4 }, vector<Signal*> {&S7} };
	B15.useNoise(shotNoise);
	B15.setResponsivity(responsivity);

	Photodiode B16{ vector<Signal*> { &S5, &S6 }, vector<Signal*> {&S8} };
	B16.useNoise(shotNoise);
	B16.setResponsivity(responsivity);

	Sink B17{ vector<Signal*> {&S7}, vector<Signal*> {} };
	B17.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B17.setDisplayNumberOfSamples(true);

	Sink B18{ vector<Signal*> {&S8}, vector<Signal*> {} };
	B18.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B18.setDisplayNumberOfSamples(true);

	//END DIA>



	/*
	//BalancedBeamSplitter B14{ vector<Signal*> { &EE7, &S2 }, vector<Signal*> {&S3, &S4} };
	BalancedBeamSplitter B14{ vector<Signal*> { &S1, &S2 }, vector<Signal*> {&S3, &S4} };
	B14.setTransferMatrix(transferMatrix);
	

	Photodiode B15{ vector<Signal*> { &S3, &S4 }, vector<Signal*> {&S5} };
	B15.useNoise(shotNoise);
	B15.setResponsivity(responsivity);

	Sampler B16{ vector<Signal*> { &S5 }, vector<Signal*> { &S6 } };
	
	BitDecider B17{ vector<Signal*> { &S6 }, vector<Signal*> { &S7 } };

	// Comparação entre BOB e ALICE. A resposta da comparação é carregada no vector S8.
	// 1-> iguais, 0->diferentes.
	BitErrorRate B18{ vector<Signal*> { &S0, &S7 }, vector<Signal*> { &S8 } };

	Sink B19{ vector<Signal*> {&S8}, vector<Signal*> {} };
	B19.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B19.setDisplayNumberOfSamples(true);

	*/

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	// [DIA] Teste original
	//System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B11, &B12, &B13, &B14, &B15, &B16, &B17, &B18, &B19} };

	
	// [DIA] Alice e Bob
	System MainSystem{ vector<Block*> {&B1, &B13, &B14, &B15, &B16, &B17, &B18} };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}