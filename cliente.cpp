#include "cliente.h"

Cliente::Cliente(void) {
//	this->server = false;
	this->rs = new RawSocket();
}

int Cliente::getMySocket(void) {
//	return this->sock;
}

int main ( )
{
	using namespace std;
	Cliente *cliente = new Cliente();
//	cout << cliente->getMySocket() << endl;

	
	return 0;
}
