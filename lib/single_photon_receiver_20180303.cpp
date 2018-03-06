#include "single_photon_receiver_20180303.h"

SinglePhotonReceiver::SinglePhotonReceiver(vector<Signal*> &inputSignal, vector <Signal*> &outputSignal) : SuperBlock(inputSignal, outputSignal) {

	

	B1.initializeBlock(vector<Signal*>{inputSignals[0]}, vector<Signal*>{&SPR01});
	B1.setFilterType(Square);
	B1.setImpulseResponseLength(16);
	B1.setPulseWidth(0.9);

	B2.initializeBlock(vector<Signal*>{inputSignals[1]}, vector<Signal*>{&SPR02});
	B2.setFilterType(Square);
	B2.setImpulseResponseLength(16);
	B2.setPulseWidth(0.9);

	B3.initializeBlock(vector<Signal*>{}, vector<Signal*>{&SPR04});
	B3.setClockPeriod(0.001);
	B3.setSamplingPeriod(0.001 / 16);
	B3.setClockPhase(3 * PI / 2);
	
	B4.initializeBlock(vector<Signal*>{&SPR01,&SPR02, &SPR04}, vector<Signal*>{&SPR03});

	B5.initializeBlock(vector<Signal*>{&SPR03}, vector<Signal*>{&SPR05});
	B5.setFilterType(Square);
	B5.setImpulseResponseLength(16);
	B5.setPulseWidth(1);

	B6.initializeBlock(vector<Signal*>{&SPR05}, vector<Signal*>{&SPR06});
	
	B7.initializeBlock(vector<Signal*>{&SPR06}, vector<Signal*>{&SPR06_out});


	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7 });
};

