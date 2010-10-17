#include "message.h"

Message::Message(byte *msg) {
	/* Usado para contruir mensagens a partir de dados. */
	if ( isMessage(msg) )
		cloneMessage(msg,MAX_MESSAGE_SIZE);
	/* Usado para copiar mensagens na integra. */
	else
		this->setMessage(msg, (size_t) strlen( (const char*)msg));
}

void Message::cloneMessage(byte *msg, size_t msgSize)
{
	size_t i;
	for (i= 0; i < msgSize; i++)
		this->messageString[i] = msg[i];
}

int Message::setMessage(byte *msg, size_t dataSize) {
    this->messageString[0] = byte(BEGIN_MARKER);
    int i;
	if ( setMessageLength(dataSize) == -1 ) return -1;

	/* Parte de dados do buffer é mapeado para */
    for(i=0; i<dataSize; i++)
        this->messageString[i+4] = msg[i];

    this->generateParit();
	return 0;
}

/* Verifica se uma seqüência de bytes qualquer é uma mensagem. */
bool Message::isMessage(byte *data) {
    if (data[0] == byte(BEGIN_MARKER) )
        return true;
    return false;
}

void Message::printMessage(void) {
    size_t i;
    size_t tam = this->getMessageLength();
    for(i=0; i < tam; i++)
        cout << this->messageString[i+4];
    cout << endl;
    cout << "Paridade = " << this->messageString[MAX_MESSAGE_SIZE-1] << endl;
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

void Message::setParit(byte parit)
{
    this->messageString[MAX_MESSAGE_SIZE-1] = parit;
}

byte Message::getParit()
{
    return this->messageString[MAX_MESSAGE_SIZE-1];
}

/* Retorna se esta mensagem é valida. */
bool Message::messageValida() {
    if (this->messageString[0] != byte(BEGIN_MARKER) )
        return false;
    size_t i;
    size_t size = this->getMessageLength();
    byte parit[2];
    parit[0] = parit[1] = 0;

    /* Checa paridade de 16 bits */
    for(i=0; i < size; i++)
        if((i%2) == 0)
            parit[0] ^= this->messageString[i+4];
        else
            parit[1] ^= this->messageString[i+4];

    /* Deixa paridade em 8 bits */
    parit[0] ^= parit[1];
    /* Se paridade calculada == paridade da mensagem enviada */
    return (parit[0] == this->getParit()) ? true : false;
}

void Message::generateParit()
{
    int i;
    size_t size = this->getMessageLength();
    byte parit[2];
    parit[0] = parit[1] = 0;

    /* Gera paridade de 16 bits */
    for(i=0; i < size; i++)
        if((i%2) == 0)
            parit[0] ^= this->messageString[i+4];
        else
            parit[1] ^= this->messageString[i+4];

    /* Gera paridade em 8 bits */
    parit[0] ^= parit[1];

    this->setParit(parit[0]);
    cout << "Gerou paridade = " << this->messageString[MAX_MESSAGE_SIZE-1] << endl;
}


