#ifndef H_CLIENTE_H
#define H_CLIENTE_H

#include "socket.h"
#include <iostream>

class Cliente {
	private:
		bool server;
		RawSocket *rs;

	public:
		Cliente(void);
		int getMySocket(void);
};

#endif

