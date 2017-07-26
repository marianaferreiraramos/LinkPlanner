#include "netxpto.h"
#include "binary_source.h"
#include "discrete_to_continuous_time.h"
#include "sink.h"

#include <fstream>
#include <iostream>

using namespace std;

int main() {

	//Input parameters

	int numberOfBitsReceived(-1);
	int numberOfBitsGenerated = 5;
	double bitPeriod = 1.0 / 50e9;

	//int prbsPatternLength = 5;

	int samplesPerSymbol = 5;
	double symbolPeriod = bitPeriod / samplesPerSymbol;

	t_integer bufferLength = 3;

	//Signal declaration and initialization

	Binary S0{ "S0.sgn" }; //output signal of the binary source
	S0.setBufferLength(bufferLength);
	S0.setFirstValueToBeSaved(true); 

	TimeContinuousAmplitudeDiscreteReal S1{ "S1.sgn" };
	S1.setBufferLength(bufferLength);
	S1.setFirstValueToBeSaved(true);

	//Blocks declaration and initialization

	BinarySource B1{ vector<Signal *> {}, vector<Signal *> { &S0 } };
	B1.setNumberOfBits(numberOfBitsGenerated);
	B1.setBitPeriod(bitPeriod);
	B1.setMode(DeterministicAppendZeros);
	B1.setBitStream("01");
	//B1.setPatternLength(prbsPatternLength);

	DiscreteToContinuousTime B2{ vector<Signal *> { &S0 }, vector<Signal *> { &S1 } };
	B2.setNumberOfSamplesPerSymbol(samplesPerSymbol);

	Sink B3{ vector<Signal *> { &S1 }, vector<Signal *> {} };
	B3.setDisplayNumberOfSamples(numberOfBitsReceived);
	B3.setNumberOfSamples(numberOfBitsGenerated);

	//System configuration

	System MainSystem{ vector<Block*> { &B1, &B2, &B3 } };

	MainSystem.run();


	return 0;
}