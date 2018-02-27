#include "message_processor_alice_20180205.h"

void MessageProcessorAlice::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[1]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[1]->setSamplesPerSymbol(inputSignals[1]->getSamplesPerSymbol());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
}



bool MessageProcessorAlice::runBlock(void) {

	bool process{ false }, alive{ false };

	do {
		process = processStoredMessages();
		alive = alive || process;
		process = processInMessages();
		alive = alive || process;
	} while (process);

	return alive;
}

bool MessageProcessorAlice::processStoredMessages() {

	bool alive{ false };

	for(auto n = 0; n < numberOfStoredMessages; n++) {

		t_message_type mType = getMessageType(storedMessages[n]);
		t_message_data_length mDataLength = getMessageDataLength(storedMessages[n]);
		t_message_data mData = getMessageData(storedMessages[n], mDataLength);

		string mDataOut{ "" };
		int process{ 0 };
		switch (mType) {

			case BasisReconciliation:

				int ready = min(inputSignals[0]->ready(), mDataLength);
			//				ready = min(ready, outputSignals[0]->space());
				process = min(ready, outputSignals[0]->space());

				if (process <= 0) return alive;

				for (auto k = 0; k < process; k++) {

					alive = true;

					t_binary inBit;
					inputSignals[0]->bufferGet(&inBit);

					if (inBit == mData[k]) {
						outputSignals[0]->bufferPut((t_binary)1);
						mDataOut.append("1");
					}
					else {
						outputSignals[0]->bufferPut((t_binary)0);
						mDataOut.append("0");
					}
				}
				break;
		}

		int dLength = mDataLength - process;
		mData.erase(mData.begin(), mData.begin() + process);
		if (dLength == 0) {
			storedMessages.erase(storedMessages.begin() + n);
			numberOfStoredMessages = (int)storedMessages.size();
		}
		else {
			storedMessages[n].messageDataLength = to_string(dLength);
			string mDataUpdated{ "" };
			for (unsigned int m = 0; m < mData.size(); m++) {
				mDataUpdated.append(to_string(mData[m]));
			}
			storedMessages[n].messageData = mDataUpdated;
		}

		t_message messageOut;
		messageOut.messageType = mType;
		messageOut.messageDataLength = to_string((int)mDataOut.size());
		messageOut.messageData = mDataOut;

		if (mDataOut.size() != 0)
		{
			outputSignals[1]->bufferPut((t_message)messageOut);
		}
		else;

	}
	
	return alive;
}


bool MessageProcessorAlice::processInMessages() {

	bool alive{ false };

	int ready = inputSignals[1]->ready();
	if (ready > 0) {
		if (numberOfStoredMessages < maxNumberOfStoredMessages) {
			t_message msgIn;
			inputSignals[1]->bufferGet(&msgIn);
			storedMessages.push_back(msgIn);
			numberOfStoredMessages++;
			alive = true;
		}
	}

	return alive;

}


t_message_type MessageProcessorAlice::getMessageType(const t_message& msg) {

	if ((msg.messageType).compare("BasisReconcilitation")==0) return BasisReconciliation;

	return (t_message_type) 0;
}

t_message_data_length MessageProcessorAlice::getMessageDataLength(const t_message& msg) {
	if ((msg.messageDataLength).size() != 0)
		return stoi(msg.messageDataLength);
	else
		return 0;
}


t_message_data MessageProcessorAlice::getMessageData(const t_message& msg, t_message_data_length dataLength) {

	string mDataString = msg.messageData;

	vector <int> mDataVector;

	for (int k = 0; k < dataLength; k++) {
		char data = mDataString.at(k);
		
		if(data == '1')
			mDataVector.push_back(1);
		else if (data == '0')
			mDataVector.push_back(0);
		else if (data == '-') {
			char aux_data = mDataString.at(k + 1);
			if (aux_data == '2') {
				mDataVector.push_back(-2);
				k++;
			}
			else if (aux_data == '1') {
				mDataVector.push_back(-1);
				k++;
			}
		}
	}

	return mDataVector;
}

