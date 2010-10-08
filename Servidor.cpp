#include "servidor.h"

Servidor::Servidor(void) {
//	this->server = false;
	this->rs = new RawSocket();
}

int Servidor::getMySocket(void) {
//	return this->sock;
}

int main ( )
{
	using namespace std;
	Servidor *servidor = new Servidor();
//	cout << cliente->getMySocket() << endl;

	
	return 0;
}
