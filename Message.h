#ifndef H_MESSAGE_H
#define H_MESSAGE_H

#define MAX_MESSAGE_SIZE
#define INIT 126

#include <iostream>

using namespace std;

typedef unsigned char byte;

class Mensagem {
	private:
		byte messageString[MAX_MESSAGE_SIZE];
	public:
		mandarMensagem(byte *dados);

};

#endif

