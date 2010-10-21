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



	if ( ! (fp = popen(cmd.c_str(),"r") ) ) return false;

// 	teste(fp); /* Imprime o comando que deve ser redirecionado. */
	//this->ct->sendAnswer(TYPE_Y);
	this->ct->sendUntilZ(TYPE_X,fp);


	pclose(fp);

	return true;

}

void Servidor::teste( FILE *fp)
{
	char buffer[100];


	cout << "--------------------------------------------------------------------------------------------" << endl;

     while ( ! feof (fp) )
	 {
	     fgets (buffer , 100 , fp);
	     fputs (buffer , stdout);
	 }
//	 rewind(fp);
	cout << "--------------------------------------------------------------------------------------------" << endl;
}




int main ( )
{
	using namespace std;
	Servidor *servidor = new Servidor();
    Message *msg;
    byte buffer[MAX_MESSAGE_SIZE];
    while(1)
    {
		if( msg = servidor->getControl()->escuta() )
		{
			//msg->printMessage();
			if ( msg->getMessageType() == TYPE_L ) 
				servidor->cmdLS(msg);
			else if ( msg->getMessageType() == TYPE_N ) 
				cout << "Tipo N" << endl;
			else
			{

				cerr << "Tipo desconhecido: " << msg->getMessageType()  << endl;
			}
        }
	}
	return 0;
}

