#include "optical_fiber_20180319.h"

void OpticalFiber::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool OpticalFiber::runBlock(void) {
	bool alive{ false };
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return alive;

	else {
		for (auto k = 0; k < process; k++) {
			t_complex_xy inPhoton;
			inputSignals[0]->bufferGet(&inPhoton);

			t_complex inValueX = inPhoton.x;
			t_complex inValueY = inPhoton.y;
			t_complex valueXOut{ 0 };
			t_complex valueYOut{ 0 };

			double w{ 0.0 };
			switch (sopType) {
			case DeterministicStatic:
				valueXOut = cos(tetha * PI / 180)* inValueX + sin(tetha * PI / 180)* inValueY;
				valueYOut = -sin(tetha * PI / 180)* inValueX + cos(tetha * PI / 180)* inValueY;
				break;
			case DeterministicDynamic:
				w = (2 * PI * c)/lambda;
				valueXOut = cos((w * (k + 1))* PI / 180)* inValueX + sin((w * (k + 1))* PI / 180)* inValueY;
				valueYOut = -sin((w * (k + 1))* PI / 180)* inValueX + cos((w * (k + 1))* PI / 180)* inValueY;
				break;
			default:
				break;
			}

			t_complex_xy outPhoton = { valueXOut, valueYOut };
			outputSignals[0]->bufferPut(outPhoton);
			alive = true;
		}
		
	}

	return alive;
}
