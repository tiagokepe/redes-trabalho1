#include "cliente.h"

Cliente::Cliente(void) {
	this->ct = new Control();
}




int main ( )
{
	char entrada[LINE_MAX];
    Cliente *cliente = new Cliente();

	while( ( cout << "$ " )  && ( cin.getline(entrada,LINE_MAX) ) )
		cliente->interpreter(entrada);


    return 0;
}




void Cliente::interpreter(char *entrada)
{
	char *cmd = strsep(&entrada," ");

	char *opcoes = (entrada == NULL)?strdup(""):entrada;

//	cout << entrada << endl;
	if( !strcmp(cmd,"ls") )
	{
//		cout << "Comando ls" << endl;
		cmdLS(opcoes);
	}
	else if ( !strcmp(cmd,"cd") )
	{
		cout << "Comando cd" << endl;
	}
	else
	{
		cout << "Comando não encontrado" << endl;
	}

}

int Cliente::cmdLS( char *entrada)
{
	char * buffer;
	bool saiu = false;
	Message * response;
	this->ct->sendSingleMessage(TYPE_L, entrada);

	this->ct->receiveUntilZ(TYPE_X, buffer);

	return 0;
}
