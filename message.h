#ifndef H_MESSAGE_H
#define H_MESSAGE_H

#define MAX_MESSAGE_SIZE 257
#define BEGIN_MARKER 126

#include <string.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <list>

using namespace std;
using std::string;

typedef unsigned char byte;

typedef enum {
	TYPE_L, /* ls */
	TYPE_Y, /* ACK <num seq> */
	TYPE_N, /* NACK <num_seq> */
	TYPE_X, /* Mostrar na tela. */
	TYPE_Z, /* Fim de Tx. */

	TYPE_C, /* cd */
	TYPE_D, /* dados */
	TYPE_F, /* descritor de arquivos */
	
	TYPE_E1, /* erro 1 - dir inexistente */
	TYPE_E2, /* erro 2 - falta de permissão */
	TYPE_E3, /* erro 3 - espaço insuficiente */
    TYPE_E4, /* erro 4 - arq inexistente */
	
	TYPE_P, /* put */

	TYPE_G /* get */
} MessageType;



class Message
{
    public:
       Message(byte *msg = NULL);
	   Message(byte *msg, MessageType mt, int seq, int tamdados);
       byte *getMessageString(void) { return this->messageString; };
       int messageValida(void);
       void printMessage(void);
	   size_t getMessageLength();
	   byte *getMessageData();
       int setMessage(byte *msg, size_t dataSize, MessageType mt, int seq);
	   MessageType getMessageType();
	   void setParit(byte parit);
       byte getParit();
	   int getMessageSequence();
    private:
	   bool checkParity();
       byte messageString[MAX_MESSAGE_SIZE];
	   int setMessageLength(size_t dataSize);
	   void setMessageType(MessageType mt);
	   void setMessageSequence(int seq);
	   void cloneMessage(byte *msg);
	   void generateParit();
	   bool valida; /* Usado em casos onde a o setMessage não pôde construir a mensagem, messageValida usa seu valor. */
};

#endif

