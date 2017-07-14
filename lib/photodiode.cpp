#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto.h"
#include "photodiode.h"


void Photodiode::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


<<<<<<< HEAD
bool Photodiode::runBlock(void){

<<<<<<< HEAD
	//DIA>
	//turn off output
	//ofstream myfile2;
	//myfile2.open("translate.txt", fstream::app);
	//END DIA>


	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod ();
	int samplesPerSymbol = (int)round(symbolPeriod / samplingPeriod);
=======
	double samplingPeriod = inputSignals[0]->getSamplingPeriod();

	int samplesPerSymbol = inputSignals[0]->getSamplesPerSymbol();
>>>>>>> develop

	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);
=======

bool Photodiode::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);
>>>>>>> AnaLuisa

	int space = outputSignals[0]->space();

	int process = min(ready, space);

<<<<<<< HEAD
	//DIA>
	//turn off output
	//if (process == 0){
	//	myfile2.close();
	//	return false;
	//}
	//END DIA>
=======
	if (process == 0) return false;
>>>>>>> develop

<<<<<<< HEAD
	normal_distribution<double> distribution(0, 1);
	double w1 = inputSignals[0]->getCentralFrequency();
	double w2 = inputSignals[1]->getCentralFrequency();
	frequencyMismatch = abs(w1 - w2);
	double noiseAmp1;
	double noiseAmp2;

	double wavelength = inputSignals[0]->getCentralWavelength();


	unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();

	
	generatorAmp1.seed(seed);
	generatorAmp2.seed(seed);

	/*
	// Implementação de filtros gaussianos

	double amp=.5*sqrt(6.4078e-13*5);
	
	
	vector<t_real> gauss;
	gauss.resize(samplesPerSymbol);

	for (int i = 0; i < samplesPerSymbol; i++)
	{
		t_real time = -samplesPerSymbol / 2 * samplingPeriod + i*samplingPeriod;
		gauss[i] = amp*exp(-time*time / (samplingPeriod*samplingPeriod / 36));
	}

	
	if (firstPass)
	{
		firstPass = false;
		for (int i = 0; i < samplesPerSymbol * 8 - samplesPerSymbol / 2; i++)
		{
			t_complex input1;
			inputSignals[0]->bufferGet(&input1);
			t_complex input2;
			inputSignals[1]->bufferGet(&input2);


			t_real power1 = abs(input1) * abs(input1) * 4;
			t_real power2 = abs(input2) * abs(input2) * 4;
			t_real current1 = responsivity * power1;
			t_real current2 = responsivity * power2;
			t_real out = current1 - current2;


			if (out != 0)
			{
				cout << "ERRO: photodiode.cpp (expected output is 0)" << "\n";
				return false;

			}
		}
	}
	*/

	// [DIA] new
	// Power constant for shotNoise.
	t_real P = PLANCK_CONSTANT*SPEED_OF_LIGHT / (samplingPeriod*wavelength);
	// end new

	for (int i = 0; i < process; i++) {

		noiseAmp1 = distribution(generatorAmp1);
		noiseAmp2 = distribution(generatorAmp2);

<<<<<<< HEAD
		t_complex input1;
		inputSignals[0]->bufferGet(&input1);
		t_complex input2;
		inputSignals[1]->bufferGet(&input2);

		/*
		// Incio de implementação de beat frequency.
=======
=======
	/*t_real radius = 0.0003; // radius of sensor
	t_real E0 = 8.854187817e-12;
	t_real n = 1.1;*/

	t_complex inputSignal1;
	t_complex inputSignal2;
>>>>>>> AnaLuisa

	for (int i = 0; i < process; i++) {

<<<<<<< HEAD
>>>>>>> develop
		t_real powerSignal1 = gauss[aux]; // Assuming Signal input in PIN1
		t_real powerSignal2 = abs(input1 - input2) / sqrt(2); // Assuming Signal input in PIN2
		*/

		//DIA> noise old implementation
		// The 4 factor is compensating the bandpass signal representation amplitude correction.
		t_real power1 = abs(input1)*abs(input1) * 4;
		t_real power2 = abs(input2)*abs(input2) * 4;
		//END DIA>

		//DIA> noise new implementation (TEST)
		//t_real n1 = (abs(input1)*abs(input1) * 4)/P;
		//t_real n2 = (abs(input2)*abs(input2) * 4)/P;
		//END DIA>
	


		/*
		// Incio de implementação de beat frequency.
		double phaseDifference = 0;
		double arg;
		if (powerSignal1!=0)
		{
			arg = out / (8 * powerSignal1 * powerSignal2);
			if (arg<-1)
			{
				phaseDifference = PI;
			} else if (arg>1) {
				phaseDifference = 0;
			}
			else
			{
				phaseDifference = (acos(arg));
			}
		}
		*/

		//t_real sqrt_n1;
		//t_real sqrt_n2;

		if (shotNoise)
		{
			
			
			//DIA> noise old implementation
			power1 += sqrt(P)*noiseAmp1*(sqrt(power1) + sqrt(P)*noiseAmp1 / 4);
			power2 += sqrt(P)*noiseAmp2*(sqrt(power2) + sqrt(P)*noiseAmp2 / 4);
			//END DIA>


			//DIA> noise new implementation
			//poisson_distribution<int> distribution1(n1+0.001);
			//poisson_distribution<int> distribution2(n2+0.001);
			//n1 = distribution1(generatorAmp1);
			//n2 = distribution2(generatorAmp2);
			//END DIA>

		}


		/*
		// Início de implementação de beat frequency.
		if (frequencyMismatch != 0){
 			out = powerSignal1*powerSignal2*(cos(phaseDifference)*cos(frequencyMismatch*t) - sin(phaseDifference)*sin(frequencyMismatch*t));
			if (1/samplingPeriod < frequencyMismatch/PI )
			{
				cout << "ERRO: photodiode.cpp (Nyquist frequency not respected, aliasing possible)" << "\n";
				return false;
			}
		}
		*/


		//DIA> noise new implementation
		//t_real power1 = n1*P;
		//t_real power2 = n2*P;
		//END DIA>

		t_real current1 = responsivity*power1;
		t_real current2 = responsivity*power2;
		t_real out = current1 - current2;


		outputSignals[0]->bufferPut(out);
<<<<<<< HEAD

		//DIA>
		//turn off output
		//myfile2 << out << "\n";
		//END DIA>

=======
>>>>>>> develop
		t = t + samplingPeriod;
		aux++;
		if (aux==samplesPerSymbol)
		{
			aux = 0;
		}



=======
			inputSignals[0]->bufferGet(&inputSignal1);
			inputSignals[1]->bufferGet(&inputSignal2);

			t_real power1 = abs(inputSignal1)*abs(inputSignal1)*2; 
			t_real current1 = responsivity * power1;

			t_real power2 = abs(inputSignal2)*abs(inputSignal2)*2;
			t_real current2 = responsivity * power2;

			t_real outputSignal = current1 - current2;

			outputSignals[0]->bufferPut(outputSignal);

		
>>>>>>> AnaLuisa
	}
	return true;
}