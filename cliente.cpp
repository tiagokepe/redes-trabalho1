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
	else if( !strcmp(cmd, "lls") )
	{
	    cmdLLS(opcoes);
	}
	else if( !strcmp(cmd, "lcd") )
	{
	    cmdLCD(opcoes);
	}
	else if( !strcmp(cmd, "get") )
	{
		cmdGET(opcoes);
	}
	else if( !strcmp(cmd, "put") )
	{
		cmdPUT(opcoes);
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

bool Cliente::cmdGET(char *entrada)
{

	MessageType mt;
	Message *TamArq;
	do
	{
		this->ct->sendSingleMessage(TYPE_G, entrada);
		mt = this->ct->receiveAnswer();
	}
	while ( ( mt != TYPE_Y ) && ( mt != TYPE_E1 ) && ( mt != TYPE_E2 ) );

	if ( mt == TYPE_E1 )
	{
		cerr << "Arquivo inexistente." << endl;
		return false;
	}
	else if ( mt == TYPE_E2 )
	{
		cerr << "Permissão negada." << endl;
		return false;
	}

	/* Recebe Tamanho do arquivo. Não é necessario responder. A receiveSingleMessage deve tratar disso. */
        TamArq = this->ct->receiveSingleMessage(TYPE_F);
	if ( !TamArq ) return false;

	if ( TamArq->getMessageType() == TYPE_E3 )
	{
		cerr << "Espaço insuficiente" << endl;
		return false;
	}

	this->ct->receiveUntilZ(TYPE_D,basename(entrada));

	return true;
}
bool Cliente::cmdPUT(char *entrada)
{
	MessageType mt;
	FILE *fp;
	char *file;
	struct stat st;
	char tam[MAX_MESSAGE_SIZE-5];
	
	file = entrada;

	/* Tenta abrir arquivo. */
	if ( !(fp = fopen(file,"r") ) )
	{
		if(errno == ENOENT ) 
			cerr << "Arquivo não encontrado." << endl;
		else if(errno == EACCES )
			cerr << "Você não tem permissão de leitura." << endl;

		return false;
	}
	
	
	do
	{
		this->ct->sendSingleMessage(TYPE_P, entrada);
		mt = this->ct->receiveAnswer();
	}
	while ( ( mt != TYPE_Y ) &&  ( mt != TYPE_E2 ) );
	
	if( mt != TYPE_Y ) return false;

	/* Pega tamanho do arquivo. */
	stat(file,&st);
	sprintf(tam,"%d", (int ) st.st_size);

	/* Envia a descrição do arquivo para o servidor. */
	do
	{
		this->ct->sendSingleMessage(TYPE_F,tam);
		mt = this->ct->receiveAnswer();
	}while ( ( mt != TYPE_Y ) && ( mt != TYPE_E3 ) );

	if ( mt == TYPE_E3 )
	{
		cerr << "O destino não possui espaço suficiente." << endl;
		return NULL;
	}

	this->ct->sendUntilZ(TYPE_D,fp);

	fclose(fp);
}

void Cliente::cmdCD(char *entrada)
{
    byte resposta;
 	/* Aguarda resposta Ack para enviar os dados */
    do 
    {
        this->ct->sendSingleMessage(TYPE_C, entrada);
        resposta = this->ct->receiveAnswer();
    } while ( resposta != TYPE_Y && resposta != TYPE_E1 && resposta != TYPE_E2 );

    if (resposta == TYPE_E1)
        cerr << "Error: Diretório inexistente." << endl;
    else 
        if (resposta == TYPE_E2)            
            cerr << "Error: Permissão negada." << endl;
  
}

void Cliente::cmdLLS(char *opcoes)
{
    string cmd ("ls ");
    string opt (opcoes);    
    cmd += opt;
    system(cmd.c_str());
    
}

void Cliente::cmdLCD(char *opcoes)
{
    chdir(opcoes);
    
}
