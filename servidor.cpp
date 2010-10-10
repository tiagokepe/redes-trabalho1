#include "servidor.h"
#include "message.h"

Servidor::Servidor(void) {
//	this->server = false;
	this->sock = new RawSocket();
}

RawSocket *Servidor::getMySocket(void) {
	return this->sock;
}

int main ( )
{
	using namespace std;
	Servidor *servidor = new Servidor();
    Message *msg;
    byte buffer[MAX_MESSAGE_SIZE];
    while(1)
    {
		if( msg = servidor->getMySocket()->getMessage() )
			if ( msg->messageValida() ) {
                msg->printMessage();
            }
    }


	return 0;
}
