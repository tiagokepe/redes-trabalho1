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
        cmdCD(opcoes);
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
	/* Aguarda resposta Ack para enviar os dados */
    do 
    {
        this->ct->sendSingleMessage(TYPE_L, entrada);
        //cout << "Cliente no do" << endl;
    }
    while ( this->ct->receiveAnswer() != TYPE_Y );

    /* Envia os dados até a mensagem tipo Z */
	this->ct->receiveUntilZ(TYPE_X, buffer);

	return 0;
}

void Cliente::cmdCD(char *entrada)
{
	/* Aguarda resposta Ack para enviar os dados */
    do 
    {
        this->ct->sendSingleMessage(TYPE_C, entrada);
    } while ( this->ct->receiveAnswer() != TYPE_Y );
}
