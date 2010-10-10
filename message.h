#ifndef H_MESSAGE_H
#define H_MESSAGE_H

#define MAX_MESSAGE_SIZE 257
#define BEGIN_MARKER 126

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <list>

using namespace std;
using std::string;

typedef unsigned char byte;

class Message
{
    public:
       Message(byte *msg = NULL);
       byte *getMessageString(void) { return this->messageString; };
       bool messageValida(void);
       void printMessage(void);
	   size_t getMessageLength();
	   bool isMessage(byte *data);
    private:
       int setMessage(byte *msg, size_t dataSize);
       byte messageString[MAX_MESSAGE_SIZE];
	   int setMessageLength(size_t dataSize);
	   void cloneMessage(byte *msg, size_t msgSize);
};

#endif

