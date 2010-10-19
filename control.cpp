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
	int i, responseTimeout;


	for ( i = 0; ( i < MAX_TRIES )  ; i++ )
	{

	}
}



int Control::sendSingleMessage(MessageType TM, char *dados) {
    cout << "Single" << endl;
	Message *NewM = new Message( (byte *) dados,TM, this->getSequence());

	if ( ! NewM->messageValida() )
		return -1;

	this->rs->sendMessage(NewM);
	incrementSequence();

	return 0;
}

int Control::sendSingleMessage(MessageType TM, FILE *fp) {
	char buffer[MAX_MESSAGE_SIZE-5];

	if ( feof(fp ) ) return -1;

	fgets(buffer,MAX_MESSAGE_SIZE-5,fp);

	
	Message *NewM = new Message( (byte *) buffer,TM,this->getSequence());

	if ( ! NewM->messageValida() )
		return -1;

	this->rs->sendMessage(NewM);
	incrementSequence();

	return 0;
}

/* Recebe qualquer mensagem. */
Message * Control::receiveSingleMessage()
{
	Message *msg = this->rs->getMessage();

	if( !msg->messageValida()   ) return NULL;

	return msg;

}

/* Somente aceitamensagem do tipo mt. */
Message * Control::receiveSingleMessage(MessageType mt)
{
	Message *msg = this->receiveSingleMessage();

	if( ( msg->getMessageType() ) != mt  ) return NULL;

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
																							
