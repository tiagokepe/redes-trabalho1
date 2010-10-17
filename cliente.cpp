#include "cliente.h"

Cliente::Cliente(void) {
	this->rs = new RawSocket();
}

/*int Cliente::getMySocket(void) {
	return (int)this->rs;
} */

RawSocket *Cliente::getMySocket(void) {
    return this->rs;
}

int main ( )
{
    using namespace std;
    Cliente *cliente = new Cliente();
    Message *msg = new Message((byte*)"ABCD");
    cliente->getMySocket()->sendMessage(msg);

    return 0;
}
