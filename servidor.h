#ifndef H_SERVIDOR_H
#define H_SERVIDOR_H

#include "socket.h"
#include <iostream>

class Servidor {
	private:
		bool server;
		RawSocket *sock;

	public:
		Servidor(void);
		RawSocket *getMySocket(void);
};

#endif

