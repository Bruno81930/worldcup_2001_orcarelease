#include "client.h"
#include "communicate.h"
#include "message.h"
#include "baseConvert.h"

#define MESSAGESIZE 510

using namespace communication;
char message[MESSAGESIZE];

Base72 converter;
TheMessage currentMessage;

void model2Message()
{
    static long data[MESSAGESIZE/5];
    
    for (int i=0; i<MESSAGESIZE/5; i++)
    {
	data[i] = 0;
    }
    currentMessage.update_message_from_memory();
    int endPos = currentMessage.write_to_stream(0, data);
    // cerr<<"COM_TEST_WRITE_ENDPOS: "<<endPos<<endl;
    // cerr<<"COM_SAY_LONG_MESSAGE: ";
    for (int i=0; i<MESSAGESIZE/5; i++)
    {
	// cerr<<data[i]<<":";
    }
    // cerr<<endl;
    //cerr<<"COM_NO_ERROR 1"<<endl;
    char * tmpMsg;
    //cerr<<"COM_NO_ERROR 2"<<endl;
    for (int i=0; i<MESSAGESIZE/5; i++)
    {
	//cerr<<"COM_NO_ERROR 3"<<i<<endl;
	tmpMsg = converter.decToBase72(data[i]);
	//cerr<<"COM_NO_ERROR 4"<<i<<endl;
	for (int j=0; j<5; j++)
	{
	    message[i*5 + j] = tmpMsg[j];
	    //cerr<<"COM_NO_ERROR 5"<<i<<j<<endl;
	}
	delete[] tmpMsg;
    }
    //cerr<<"COM_NO_ERROR 6"<<message<<endl;
    //say(message);
    //cerr<<"COM_NO_ERROR 7"<<endl;
}

void message2Model(char * message)
{
}

void parse_player_message(char * msg)
{
	if (Mem->OP_communicate)
	{
		//cerr<<"COM_HEAR_MESSAGE: "<<msg<<endl;
		static long data[MESSAGESIZE/5];
		// cerr<<"COM_TEST_HEARED_LONG_MESSAGE: ";
		for (int i=0; i<MESSAGESIZE/5; i++)
		{
			data[i] = converter.base72ToDec(msg + i*5);
			// cerr<<data[i]<<":";
		}
		// cerr<<endl;
		int endPos = currentMessage.read_from_stream(0, data);
		if (endPos == 0) return;
		// cerr<<"COM_TEST_READ_ENDPOS: "<<endPos<<endl;
		currentMessage.update_memory_from_message();
		// cerr<<"COM_TEST_TEST_TEST: "<<endl;
	}
}

void communicate()
{
/** If my message is not due this cycle I don't have to worry about it. */
//    if (!Mem->MyCommunicationInterrupt()) return;
    
	//cerr<<"COM_GENERATE_MESSAGE: "<<Mem->MyNumber << " ";
	//cerr << Mem->OP_communicate << ":";
	//cerr << Mem->OP_communicate_mates << ":";
	//cerr << Mem->OP_communicate_max_mates << ":";
	//cerr << Mem->OP_communicate_opponents << ":";
	//cerr << Mem->OP_communicate_max_opponents << ":";
	//cerr << Mem->OP_communicate_ball << ":";
	//cerr << Mem->OP_communicate_strategy << ":";
	//cerr << endl;
	// folgendes wie oben, nnur in der neuen Fassung
	MAKELOG((50, 1,
		"COM_GENERATE_MESSAGE: %d %d %d %d %d %d %d",
		Mem->OP_communicate, Mem->OP_communicate_mates, Mem->OP_communicate_max_mates,
		Mem->OP_communicate_opponents, Mem->OP_communicate_max_opponents, Mem->OP_communicate_ball,
		Mem->OP_communicate_strategy
		));

	if (Mem->OP_communicate)
	{
		model2Message();
		//cerr<<"COM_SAY_MESSAGE: "<<message<<endl;

		say(message);
	}
    //cerr<<"COM_BALL_POS ("<<Mem->MyNumber<<") :"<<Mem->BallAbsolutePosition().x<<"/"<<Mem->BallAbsolutePosition().y<<endl;
}
