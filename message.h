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
   	    bool isMessage(byte *data);
        bool messageValida(void);
        void printMessage(void);

        byte *getMessageString(void) { return this->messageString; };

	    int setMessageLength(size_t dataSize);
        size_t getMessageLength();
        void setParit(byte parit);
        byte getParit();
     private:	 
        void cloneMessage(byte *msg, size_t msgSize);
        int setMessage(byte *msg, size_t dataSize);
        byte messageString[MAX_MESSAGE_SIZE];
        void generateParit();
};

#endif

