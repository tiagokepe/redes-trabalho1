#include "servidor.h"

Servidor::Servidor(void) {
//	this->server = false;
	this->ct = new Control();
}


bool Servidor::cmdLS(Message * msg)
{
	FILE *fp;
	string cmd ( "ls ");

    this->ct->sendAnswer(TYPE_Y);

	if( ( msg->getMessageLength() - 3 ) > 0 )
		cmd += (char * ) msg->getMessageData();

	if ( ! (fp = popen(cmd.c_str(),"r") ) ) return false;

	this->ct->sendUntilZ(TYPE_X,fp);

	pclose(fp);

	return true;
}
bool Servidor::cmdGET(Message *msg)
{
	FILE *fp;
	char *file;
	char tam[MAX_MESSAGE_SIZE-5];
	struct stat st;
	MessageType mt;

	file = (char * ) msg->getMessageData();

	if(  !( fp = fopen(file,"r") ) )
	{
		if( errno == ENOENT )
			this->ct->sendAnswer(TYPE_E1);
		else if ( errno == EACCES )
			this->ct->sendAnswer(TYPE_E2);

		return false;
	}
	this->ct->sendAnswer(TYPE_Y);

	stat(file,&st);

	cout << "SIZE: " << st.st_size << endl;
	sprintf(tam,"%d",st.st_size);

	do
	{
		this->ct->sendSingleMessage(TYPE_F,tam);
		mt = this->ct->receiveAnswer();
	}while ( ( mt != TYPE_Y ) && ( mt != TYPE_E3 ) );

	if ( mt != TYPE_Y ) return NULL; /* Cliente não tem espaço suficiente, não envie nada. */

	this->ct->sendUntilZ(TYPE_D,fp);//TYPE_D


	fclose(fp);

	return true;

}
bool Servidor::cmdCD(Message * msg)
{
    byte *dir;
    dir = msg->getMessageData();
    int error, errval;
//    cout << "DIr = " << dir << endl;
    errval = chdir((char *)dir);
    error = errno;
    if ( errval == 0 ) {
        this->ct->sendAnswer(TYPE_Y);
        cout << "OK" << endl;
    }        
    else
    {
        if (error == ENOENT) {
            cout << "Else 1" << endl;
            this->ct->sendAnswer(TYPE_E1);
        }            
        else
            if (error == EACCES) {
                cout << "Else 2" << endl;
                this->ct->sendAnswer(TYPE_E2);
            }
    }                
                     
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
			if ( msg->getMessageType() == TYPE_L ) {
                cout << "TIPO LS" << endl;          
				servidor->cmdLS(msg);
            }
			else if ( msg->getMessageType() == TYPE_C ) 
			        servidor->cmdCD(msg);
				//cout << "Tipo N" << endl;
			else if ( msg->getMessageType() == TYPE_G )
			{
				cout << "TIPO GET" << endl;
				servidor->cmdGET(msg);
			}
			else 
			{

				cerr << "Tipo desconhecido: " << msg->getMessageType()  << endl;
			}
        }
	}
	return 0;
}

