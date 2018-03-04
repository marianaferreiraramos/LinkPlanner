#ifndef SINGLE_PHOTON_RECEIVER_H
#define SINGLE_PHOTON_RECEIVER_H

#include "netxpto_20180118.h"
#include "single_photon_detector_20180206.h"
#include "pulse_shaper_20180111.h"
#include "sampler_20171119.h"
#include "coincidence_detector_20180206.h"
#include "super_block_interface_20180118.h"
#include "sink_20180118.h"
#include "clock_20171219.h"

class SinglePhotonReceiver : public SuperBlock{ 

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	TimeContinuousAmplitudeContinuousReal SPR01{ "SPR01.sgn" }; // single photon detector output
	TimeContinuousAmplitudeContinuousReal SPR02{ "SPR02.sgn" }; //shapper
	TimeContinuousAmplitudeContinuousReal SPR03{ "SPR03.sgn" }; // single photon detector output
	TimeContinuousAmplitudeContinuousReal SPR04{ "SPR04.sgn" }; //clock
	TimeContinuousAmplitudeContinuousReal SPR05{ "SPR05.sgn" }; //sampler
	TimeDiscreteAmplitudeDiscreteReal SPR06{ "SPR06.sgn" }; //sampler
	TimeContinuousAmplitudeContinuousReal SPR07{ "SPR07.sgn" };
	TimeDiscreteAmplitudeDiscreteReal SPR06_out{ "SPR06_out.sgn" };
	TimeContinuousAmplitudeContinuousReal SPR08{ "SPR08.sgn" };

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################
	PulseShaper B1;
	PulseShaper B2;
	Clock B3;
	CoincidenceDetector B4;
	PulseShaper B5;
	Sampler B6;
	SuperBlockInterface B7;

	Sink B9;
	

public:

	/* Input Parameters */
	double DetectorProbabilityDarkCount{ 0.001 };
	int samplesToSkip{ 0 };
	PulseShaperFilter filterType{ Square };
	int pathH{ 0 };
	int pathV{ 1 };

	/*Methods*/
	SinglePhotonReceiver(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals);

	/*Set Methods*/
	void setPathDetectorH(int pad) { pathH = pad; };
	void setPathDetectorV(int pad) { pathV = pad; };
	void setPulseShaperFilter(PulseShaperFilter fType) { B2.setFilterType(fType); B5.setFilterType(fType); };
};

#endif
