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
	TYPE_E, /* erro */

	TYPE_P, /* put */

	TYPE_G /* get */
} MessageType;



class Message
{
    public:
       Message(byte *msg = NULL);
	   Message(byte *msg, MessageType mt, int seq);
       byte *getMessageString(void) { return this->messageString; };
       bool messageValida(void);
       void printMessage(void);
	   size_t getMessageLength();
	   byte *getMessageData();
	   bool isMessage(byte *data);
       int setMessage(byte *msg, size_t dataSize, MessageType mt, int seq);
	   MessageType getMessageType();
	   void setParit(byte parit);
           byte getParit();
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

