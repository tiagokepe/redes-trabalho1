#ifndef H_CONTROL_H
#define H_CONTROL_H

#include "socket.h"
#include <iostream>
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <string.h>


#define MAX_TRIES 16
#define MAX_SEQ 7
#define TIMEOUT 60

class Control {
	private:
		bool server;
		int sequence;
		RawSocket *rs;
		void incrementSequence();
		void decrementSequence();
		int waitTimeout();
		bool diskspace(Message *msg); /* Retorna verdadeiro se houver espaço em disco. */
		int seqEsperada;
	public:
		Message * escuta();
		int sendSingleMessage(MessageType mt, char *data, int tamdados); /* Dados da mensagem estão em uma string. */
		int sendSingleMessage(MessageType mt, char *data); /* Somente para dados em formato texto. */
		int sendSingleMessage(MessageType mt, FILE *data); /* Dados da mensagem são apontados por um file pointe. */
        void sendSingleMessage(MessageType TM);
		Message *receiveSingleMessage(MessageType mt);
		Message *receiveSingleMessage();
		bool receiveUntilZ(MessageType mt,char *data);
		bool sendUntilZ(MessageType mt, FILE *fp);
		MessageType receiveAnswer();
		bool sendAnswer(MessageType tipo_resposta );
		int getSequence();
		Control(void);
        RawSocket *getMySocket(void);
};

#endif

