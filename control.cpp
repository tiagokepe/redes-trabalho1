#include "control.h"

Control::Control(void) {
	this->rs = new RawSocket();
	this->sequence = 0;
	this->seqEsperada = -1;
}

RawSocket *Control::getMySocket(void) {
    return this->rs;
}

/* Incrementa sequencia que conta mensagens. */
void Control::incrementSequence() {
	this->sequence = (this->sequence + 1)%MAX_SEQ;
}

/* Pega Seqüência da mensagem. */
int Control::getSequence() {
	return this->sequence;
}

/* Recebe mensagens do tipo TM até receber uma mensagem Z, indicando final. */
bool Control::receiveUntilZ(MessageType TM, char *dados)
{
	Message *msg;
	bool fim_envio = false;

	do
	{
		if ( !( msg = receiveSingleMessage(TM) ) ) return NULL;

		if ( msg->getMessageType() == TYPE_X )
		{
	            cout << "Enviando resposta Y" << endl;
			msg->printMessage();
		}
		else if ( msg->getMessageType() == TYPE_Z )
		{
			cout << "Finalizou mensagem" << endl;
			fim_envio = true;
		}
	} while( !fim_envio );
	return true;
}

/* Manda várias mensagens até mensagem Z, indicando final. */
bool Control::sendUntilZ(MessageType TM, FILE *fp )
{
	char buffer[MAX_MESSAGE_SIZE-5];

	rs->cleanBuf( (byte * ) buffer,MAX_MESSAGE_SIZE-5);

	MessageType resposta = TYPE_N;

	while ( !feof(fp) )
	{
        cout << "Entrou no While" << endl;
		fread(buffer,sizeof(char),MAX_MESSAGE_SIZE-6,fp);

		if ( buffer[0] )
			do {
                cout << "Entrou no DO***" << endl;
				sendSingleMessage(TM,buffer);
                cout << "Enviou dados" << endl;
				resposta = this->receiveAnswer();
                cout << "Resposta = " << resposta << endl;
			} while (resposta != TYPE_Y);

		rs->cleanBuf((byte * ) buffer,MAX_MESSAGE_SIZE-5);
	}
    cout << "********************Mandando a Z**********************" << endl;
    do {
    	sendSingleMessage(TYPE_Z);
		resposta = this->receiveAnswer();
    } while (resposta != TYPE_Y);

    cout << "Reposta = " << resposta << endl;
	return true;
}

void Control::sendSingleMessage(MessageType TM) {
	Message *NewM = new Message( NULL,TM, this->getSequence());

	this->rs->sendMessage(NewM);
	incrementSequence();

	if ( NewM ) ;
        //cout << NewM->getMessageType() << endl; 
		
}

/* Manda única mensagem do tipo TM que vem da sequencia de char em dados. */
int Control::sendSingleMessage(MessageType TM, char *dados) {
	Message *NewM = new Message( (byte *) dados,TM, this->getSequence());


	this->rs->sendMessage(NewM);
	incrementSequence();
    

	if ( NewM )
		//cout << "Tamnho e paridade enviados:" << NewM->getMessageLength() << " ," << NewM->getParit() << endl;
    //cout << "Tipo = " << TM << endl;
	
	return 0;
}

/* Manda única mensagem que venha de um arquivo apontado por FP. */
int Control::sendSingleMessage(MessageType TM, FILE *fp) {
	char buffer[MAX_MESSAGE_SIZE-5];
	rs->cleanBuf((byte * ) buffer,MAX_MESSAGE_SIZE-5);

	if ( feof(fp ) ) return -1;

	buffer[MAX_MESSAGE_SIZE-6] = '\0';

	Message *NewM = new Message( (byte *) buffer,TM,this->getSequence());


	this->rs->sendMessage(NewM);
	incrementSequence();

	if ( NewM )
		//cout << "Tamanho e paridade enviados:" << NewM->getMessageLength() << " ," << NewM->getParit() << endl;
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
		if ( timeout ) /* Recebeu a mensagem no tempo esperado */
		{
			msg = this->rs->getMessage();
			if ( msg ) 
			{
				//cout << "entrou no receivemsg" << endl;
				if ( msg->messageValida() == 1 ) /* Mensagem recebida. */
				{
					//cout << "entrou no receivemsg2" << endl;
					received=true;
					sendAnswer(TYPE_Y);
					this->seqEsperada =  ( msg->getMessageSequence()  + 1)%MAX_SEQ;
				}
				else if ( ( msg->messageValida() == -1 ) && (this->seqEsperada >= 0 ) ) /* Paridade erra, pede reenvio. */
					sendAnswer(TYPE_N);
			}
		}
		else //if ( this->seqEsperada >= 0 )   /* Timeout estourou, pede reenvio. */
		{
			sendAnswer(TYPE_N);
		}
	}

	if ( !(msg) || !( msg->messageValida() ) ) return NULL;
	cout << "Tipo = " << msg->getMessageType() << endl;

//	sendAnswer(TYPE_Y);
	
	return msg;

}

/* Recebe e retorna uma resposta. */
MessageType Control::receiveAnswer()
{
	int i;
	Message *msg;
	int timeout;


	for( i = 0; (i < MAX_TRIES ) ;i++)
	{
        timeout = waitTimeout();
		if ( timeout ) /* Recebeu a mensagem no tempo esperado */
		{
            msg = this->rs->getMessage();
			if ( msg ) 
			{
				if ( msg->messageValida() == 1 )
				{
					if ( msg->getMessageType() == TYPE_N )
						return TYPE_N;
					else if ( msg->getMessageType() == TYPE_Y )
						    return TYPE_Y;
					else if ( msg->getMessageType() == TYPE_E1 )
						    return TYPE_E1;
				    else if ( msg->getMessageType() == TYPE_E2 )
						    return TYPE_E2;
					else if ( msg->getMessageType() == TYPE_E3 )
						    return TYPE_E3;
					else if ( msg->getMessageType() == TYPE_E4 )
						    return TYPE_E4;      
					else
					{
						cerr << "Reposta Inválida" << endl;
						return TYPE_N;
					}
				}
			}
		}
	}
	return TYPE_N;
}

bool Control::sendAnswer(MessageType tipo_resposta )
{
	char seq[MAX_SEQ];
	
	this->rs->cleanBuf( (byte * ) seq,MAX_SEQ);

	sprintf(seq,"%d\0",this->seqEsperada);

	if ( ( tipo_resposta != TYPE_N ) && ( tipo_resposta != TYPE_Y ) 
	        && ( tipo_resposta != TYPE_E1 )  && ( tipo_resposta != TYPE_E2 )
	        && ( tipo_resposta != TYPE_E3 )&& ( tipo_resposta != TYPE_E4 ) )
		return false;
	
	cout << "ENtrou no sendAnswer" << endl;
	sendSingleMessage(tipo_resposta,seq);

	return true;
}

Message * Control::escuta()
{
	Message *msg;
	//int response = this->waitTimeout();
//	if  (response )
//	{
		if ( !( msg = this->rs->getMessage() ) ) return NULL;
		
		if ( msg->messageValida() == 1 )
		    return msg;
//	}

	return NULL;
}

/* Somente aceitamensagem do tipo mt. */
Message * Control::receiveSingleMessage(MessageType mt)
{
    int i, timeout;
	Message *msg = NULL;
	bool received = false;

	for( i = 0; (i < MAX_TRIES ) && !( received ); i++)
	{
		timeout = waitTimeout();
		if ( timeout ) /* Recebeu a mensagem no tempo esperado */
		{
			msg = this->rs->getMessage();
			if ( msg ) 
			{
				//cout << "entrou no receivemsg" << endl;
				if ( msg->messageValida() == 1 && ( ( msg->getMessageType() == mt ) || ( msg->getMessageType() == TYPE_Z   ) ) ) /* Mensagem recebida. */
				{
					//cout << "entrou no receivemsg2" << endl;
					received=true;
					sendAnswer(TYPE_Y);
					this->seqEsperada =  ( msg->getMessageSequence()  + 1)%MAX_SEQ;
				}
				else if ( ( msg->messageValida() == -1 ) && (this->seqEsperada >= 0 ) ) /* Paridade erra, pede reenvio. */
					sendAnswer(TYPE_N);
			}
		}
		else //if ( this->seqEsperada >= 0 )   /* Timeout estourou, pede reenvio. */
		{
			sendAnswer(TYPE_N);
		}
	}

	if ( !received && (!(msg) || !( msg->messageValida() ) ) ) return NULL;


	cout << "Tipo = " << msg->getMessageType() << endl;
	return msg;


/*
	Message *msg = this->receiveSingleMessage();

	if( !(msg) || ( msg->getMessageType() ) != mt  ) return NULL;

		return msg;
*/

}

/* Espera um tempo por resposta no socket. */
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


																							
