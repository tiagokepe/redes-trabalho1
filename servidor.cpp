#include "servidor.h"

Servidor::Servidor(void) {
//	this->server = false;
	this->ct = new Control();
}


bool Servidor::cmdLS(Message * msg)
{
	FILE *fp;
	string cmd ( "ls ");


	if( ( msg->getMessageLength() - 3 ) > 0 )
		cmd += (char * ) msg->getMessageData();


	cout << cmd << endl;

	if ( ! (fp = popen(cmd.c_str(),"r") ) ) return false;

	this->ct->sendSingleMessage(TYPE_X,fp);
 //	teste(fp); /* Imprime o comando que deve ser redirecionado. */


	pclose(fp);

	return true;

}

void Servidor::teste( FILE *fp)
{
	char buffer[100];

     while ( ! feof (fp) )
	 {
	     fgets (buffer , 100 , fp);
	     fputs (buffer , stdout);
	 }
}




int main ( )
{
	using namespace std;
	Servidor *servidor = new Servidor();
    Message *msg;
    byte buffer[MAX_MESSAGE_SIZE];
    while(1)
    {
		if( msg = servidor->getControl()->receiveSingleMessage() )
		{
			msg->printMessage();
			if ( msg->getMessageType() == TYPE_L ) 
				servidor->cmdLS(msg);
			else
				cerr << "Tipo desconhecido" << endl;
        }
	}


	return 0;
}

