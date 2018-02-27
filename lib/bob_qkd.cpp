#include "bob_qkd.h"

BobQKD::BobQKD(vector<Signal*> &inputSignal, vector <Signal*> &outputSignal) : SuperBlock(inputSignal, outputSignal) {
	BB0.initializeBlock(vector <Signal*> {inputSignals[0]}, vector<Signal*>{&Bob_1});

	BB1.initializeBlock(vector <Signal*> {&Bob_1}, vector<Signal*>{&Bob_2});

	BB2.initializeBlock(vector <Signal*> {&Bob_2}, vector<Signal*>{&Bob_8});

	BB3.initializeBlock(vector <Signal*> {inputSignals[1]}, vector<Signal*>{&Bob_3});
	BB4.initializeBlock(vector <Signal*> {&Bob_3}, vector<Signal*>{&CLKB_out1, &CLKB_out2});
	
	BB10.initializeBlock(vector <Signal*> {inputSignals[2], inputSignals[3]}, vector<Signal*>{&Bob_7, &Bob_11});

	BB5.initializeBlock(vector<Signal*>{&Bob_7, inputSignals[4] }, vector<Signal*>{&Bob_4, &Bob_10});

	BB7.initializeBlock(vector<Signal*>{&Bob_10, inputSignals[5]}, vector<Signal*>{&Bob_5, &C_C_2});

	BB6.initializeBlock(vector<Signal*>{&Bob_11, &Bob_5}, vector<Signal*>{&Bob_6, &Bob_9});
	Bob_6.setBufferLength(512);
	Bob_6.setSamplingPeriod(0.001);
	Bob_6.setSymbolPeriod(0.001);
	Bob_9.setBufferLength(512);
	Bob_9.setSamplingPeriod(0.001);
	Bob_9.setSymbolPeriod(0.001);

	BB11.initializeBlock(vector<Signal*>{&Bob_9}, vector<Signal*>{});
	BB11.setNumberOfSamples(500);
	BB11.setDisplayNumberOfSamples(true);

	BBSink.initializeBlock(vector<Signal*>{&Bob_4}, vector<Signal*>{});
	BBSink.setDisplayNumberOfSamples(true);

	BB9.initializeBlock(vector <Signal*> {&Bob_8, &CLKB_out1, &CLKB_out2, &C_C_2, &Bob_6}, vector<Signal*>{outputSignals[0], outputSignals[1], outputSignals[2], outputSignals[3], &Bob_6_out});

	setModuleBlocks({ &BB0, &BB1, &BB2, &BB3, &BB4, &BB10, &BB5, &BB7, &BB6, &BB11, &BBSink, &BB9 });


}
