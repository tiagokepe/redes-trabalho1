#ifndef H_SERVIDOR_H
#define H_SERVIDOR_H

#include "control.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>

class Servidor {
	private:
		bool server;
		Control *ct;

	public:
		void teste(FILE *); /* A ser removido. */
		Servidor(void);
		RawSocket *getMySocket(void);
		Control *getControl() { return this->ct; };
		bool cmdLS(Message *msg );
		bool cmdCD(Message * msg);

};

#endif

