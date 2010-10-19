#include "cliente.h"

Cliente::Cliente(void) {
	this->ct = new Control();
}




int main ( )
{
	char entrada[LINE_MAX];
    Cliente *cliente = new Cliente();
//    cliente->getMySocket()->sendMessage(msg);

	while( ( cout << "$ " )  && ( cin.getline(entrada,LINE_MAX) ) )
		cliente->interpreter(entrada);


    return 0;
}




void Cliente::interpreter(char *entrada)
{
    cout << "Interpretar" << endl;
	char *cmd = strsep(&entrada," ");

	char *opcoes = (entrada == NULL)?strdup(""):entrada;

	if( !strcmp(cmd,"ls") )
	{
		cout << "Comando ls" << endl;
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
    cout<< "ENterpretou" << endl;

}

int Cliente::cmdLS( char *entrada)
{
	Message * response;
	this->ct->sendSingleMessage(TYPE_L, entrada);

//	response = this->ct->receiveSingleMessage(TYPE_X);

//	response->printMessage();
	return 0;
}
