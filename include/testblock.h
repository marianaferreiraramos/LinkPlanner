# ifndef PROGRAM_INCLUDE_TESTBLOCK_H_
# define PROGRAM_INCLUDE_TESTBLOCK_H_

# include "netxpto.h"
# include "balanced_beam_splitter.h"
# include "photodiode.h"
# include "ti_amplifier.h"
# include "bit_decider.h"
# include "local_oscillator.h"
# include "sampler.h"
# include "delayer.h"
# include "super_block_interface.h"


// this is a test block for the purpose of beta testing new code
// current code: Building Homodyne superblock.
class testblock : public Block {

	bool firstTime{ true };

public:

	testblock() {};
	testblock(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) :Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

};

#endif
