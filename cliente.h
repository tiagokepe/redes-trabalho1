#ifndef H_CLIENTE_H
#define H_CLIENTE_H

#include "control.h"
#include <iostream>
#include <string.h>
#include <stdio.h>


/* Tamanho máximo de uma linha a ser lida */
#define LINE_MAX 1024



class Cliente {
	private:
		bool server;
		Control *ct;

	public:
		Cliente(void);
		void interpreter(char *);
		int cmdLS(char *entrada);
        void cmdCD(char *entrada);
};

#endif

