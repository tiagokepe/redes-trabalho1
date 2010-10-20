#include "control.h"

Control::Control(void) {
	this->rs = new RawSocket();
	this->sequence = 0;
}



RawSocket *Control::getMySocket(void) {
    return this->rs;
}


void Control::incrementSequence() {
	this->sequence = (this->sequence + 1)%MAX_SEQ;
}

int Control::getSequence() {
	return this->sequence;
}


bool Control::receiveUntilZ(MessageType TM, char *dados)
{
	Message *msg;

	do
	{
		if ( !( msg = receiveSingleMessage(TM) ) ) return false;

		if ( msg->getMessageType() == TYPE_X )
		{
			msg->printMessage();
		}
		else if ( msg->getMessageType() == TYPE_Z )
		{
			cout << "Finalizou mensagem" << endl;
		}
	}while( msg->getMessageType() == TYPE_Z );
	return true;
}

bool Control::sendUntilZ(MessageType TM, FILE *fp )
{
	char buffer[MAX_MESSAGE_SIZE-5];


	while ( !feof(fp) )
	{
		fgets(buffer,MAX_MESSAGE_SIZE-5,fp);
	//	fread(buffer,sizeof(char),MAX_MESSAGE_SIZE-6,fp);
		buffer[MAX_MESSAGE_SIZE-6] = '\0';
		sendSingleMessage(TM,buffer);
	}
	sendSingleMessage(TYPE_Z, (char * ) "");

}



int Control::sendSingleMessage(MessageType TM, char *dados) {
    cout << "Single" << endl;
	Message *NewM = new Message( (byte *) dados,TM, this->getSequence());


	this->rs->sendMessage(NewM);
	incrementSequence();

	cout << "Tamnho e paridade enviados:" << NewM->getMessageLength() << " ," << NewM->getParit() << endl;

	return 0;
}

int Control::sendSingleMessage(MessageType TM, FILE *fp) {
	char buffer[MAX_MESSAGE_SIZE-5];

	if ( feof(fp ) ) return -1;

		

	buffer[MAX_MESSAGE_SIZE-6] = '\0';


	
	Message *NewM = new Message( (byte *) buffer,TM,this->getSequence());


	this->rs->sendMessage(NewM);
	incrementSequence();

	cout << "Tamnho e paridade enviados:" << NewM->getMessageLength() << " ," << NewM->getParit() << endl;
	return 0;
}

/* Recebe qualquer mensagem. */
Message * Control::receiveSingleMessage()
{
	int i, timeout;
	Message *msg = NULL;
	bool received = false;

	for( i = 0; (i < MAX_TRIES )  && !( received );i++)
	{
		timeout = waitTimeout();
		if ( timeout )
		{
			msg = this->rs->getMessage();
			if ( msg )
				received=true;
		}
	}

	
	return msg;

}

/* Somente aceitamensagem do tipo mt. */
Message * Control::receiveSingleMessage(MessageType mt)
{
	Message *msg = this->receiveSingleMessage();

	if( !(msg) || ( msg->getMessageType() ) != mt  ) return NULL;

	
	return msg;

}


int Control::waitTimeout()
{
	fd_set rfds;
	struct timeval tv;
    /* Watch socket to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(this->rs->getDescriptor(), &rfds);

	/* Wait up to TIMEOUT */
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
  
	return select(this->rs->getDescriptor()+1, &rfds, NULL, NULL, &tv);
}
																							
