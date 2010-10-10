#include "message.h"

Message::Message(byte *msg) {
	
	/* Usado para contruir mensagens a partir de dados. */
	if ( isMessage(msg) )
		cloneMessage(msg,MAX_MESSAGE_SIZE);
	/* Usado para copiar mensagens na integra. */
	else
		this->setMessage(msg, (size_t) strlen( (const char*)msg));
}


int Message::setMessage(byte *msg, size_t dataSize) {
    this->messageString[0] = byte(BEGIN_MARKER);
    int i;
    cout << "Tam = " << dataSize << endl;

	if ( setMessageLength(dataSize) == -1 ) return -1;

	

	/* Parte de dados do buffer é mapeado para */
    for(i=0; i<dataSize; i++)
        this->messageString[i+4] = msg[i];

    cout << "No message" << endl;
    if ( this->messageValida() ) {
        cout << "Entrou no for" << endl;
        for(i=0; i < (int ) dataSize; i++)
            cout << messageString[i+4];
    }

    cout << endl;
    cout << "Saiu" << endl;

	return 0;
}

/* Retorna se esta mensagem é valida. */
bool Message::messageValida(void) {
    if (this->messageString[0] == byte(BEGIN_MARKER) )
        return true;
    return false;
}

/* Verifica se uma seqüência de bytes qualquer é uma mensagem. */
bool Message::isMessage(byte *data) {
    if (data[0] == byte(BEGIN_MARKER) )
        return true;
    return false;
}
void Message::printMessage(void) {
    int i;
	cout << " Tamanho: " << ( int ) this->messageString[1] << endl;;
    for(i=0; i < 2; i++)
        cout << this->messageString[i+4];
    cout << endl;
}

int Message::setMessageLength(size_t dataSize )
{
	/* Tamanho da mensagem descarta marcador de início e próprio espaço para tamanho. */ 
	if( dataSize > ( MAX_MESSAGE_SIZE - 2 ) )
		return -1;

	this->messageString[1] = (byte) dataSize;

	return 0;
}

size_t Message::getMessageLength()
{
	return ( ( size_t ) this->messageString[1] );
}

void Message::cloneMessage(byte *msg, size_t msgSize)
{
	int i;
	for (i= 0;i< (int )  msgSize;i++)
		this->messageString[i] = msg[i];
}


