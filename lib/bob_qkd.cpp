#include "bob_qkd.h"

BobQKD::BobQKD(vector<Signal*> &inputSignal, vector <Signal*> &outputSignal) : SuperBlock(inputSignal, outputSignal) {
	BB0.initializeBlock(vector <Signal*> {inputSignals[0]}, vector<Signal*>{&Bob_1});

	BB1.initializeBlock(vector <Signal*> {&Bob_1}, vector<Signal*>{&Bob_2});

	BB2.initializeBlock(vector <Signal*> {&Bob_2}, vector<Signal*>{&Bob_8});

	BB3.initializeBlock(vector <Signal*> {inputSignals[1]}, vector<Signal*>{&Bob_3});

	BB4.initializeBlock(vector <Signal*> {&Bob_3}, vector<Signal*>{&CLKB_out1, &CLKB_out2});

	BB8.initializeBlock(vector<Signal*>{inputSignals[2]}, vector<Signal*>{&Bob_7, &Bob_11});

	BBSink.initializeBlock(vector<Signal*>{&Bob_7}, vector<Signal*>{});
	BBSink.setNumberOfSamples(500);
	BBSink.setDisplayNumberOfSamples(true);
	BBSink0.initializeBlock(vector<Signal*>{&Bob_11}, vector<Signal*>{});

	BB9.initializeBlock(vector <Signal*> {&Bob_8, &CLKB_out1, &CLKB_out2}, vector<Signal*>{outputSignals[0], outputSignals[1], outputSignals[2]});

	setModuleBlocks({ &BB0, &BB1, &BB2, &BB3, &BB4, &BB8, &BBSink, &BBSink0, &BB9 });


};

