#include "message.h"

	/* Usado para contruir mensagens a partir de dados. */
Message::Message(byte *msg) {
	cloneMessage(msg,MAX_MESSAGE_SIZE);
}

	/* Usado para copiar mensagens na integra. */
Message::Message(byte *msg, MessageType mt, int seq) {
	byte * data = ( msg == NULL )?( byte * )strdup(""):msg; 
	
	if ( this->setMessage(data, (size_t) strlen( (const char *)data),mt, seq) )
	{
		this->valida = false;
		cerr << "Esta mensagem é inválida" << endl;
	}
	else
		this->valida = true;
}



int Message::setMessage(byte *msg, size_t dataSize, MessageType mt, int seq) {
    this->messageString[0] = byte(BEGIN_MARKER);
    int i;

	if( msg == NULL ) dataSize = 0;

    cout << "Tam = " << dataSize << endl;

	if ( setMessageLength(dataSize) == -1 ) return -1;

	setMessageSequence(seq);
	setMessageType(mt);
	

	/* Parte de dados do buffer é mapeado para */
    for(i=0; i<dataSize; i++)
        this->messageString[i+4] = msg[i];

    if ( this->messageValida() ) {
        for(i=0; i < (int ) dataSize; i++)
            cout << messageString[i+4];
    }

//    this->generateParit();

	return 0;
}

/* Retorna se esta mensagem é valida. */
bool Message::messageValida(void) {
    if ( (this->messageString[0] == byte(BEGIN_MARKER) ) && ( this->valida ) )
        return true;
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
    cout << "Paridade = " << (int ) this->getParit() << endl;
}

int Message::setMessageLength(size_t dataSize )
{
	/* Tamanho da mensagem descarta marcador de início e próprio espaço para tamanho. */ 
	if( dataSize > ( MAX_MESSAGE_SIZE - 2 ) )
		return -1;

	this->messageString[1] = (byte) dataSize;

	return 0;
}

void Message::setMessageType(MessageType mt )
{
	this->messageString[3] = (byte) mt;

}

void Message::setMessageSequence(int seq )
{
	this->messageString[2] = (byte) seq;
}

size_t Message::getMessageLength()
{
	return ( ( size_t ) this->messageString[1] );
}

MessageType Message::getMessageType() 
{
	return ( MessageType ) this->messageString[3];
}

byte * Message::getMessageData()
{
	if ( this->getMessageLength() -3 > 0 )
		return &this->messageString[4];
	else
		return (byte * )  NULL;
}

void Message::cloneMessage(byte *msg, size_t msgSize)
{
	int i;
	this->valida = true;
	for (i= 0;i< (int )  msgSize;i++)
		this->messageString[i] = msg[i];
}

void Message::setParit(byte parit)
{
//    this->messageString[MAX_MESSAGE_SIZE-1] = parit;
	  this->messageString[this->getMessageLength()+1] = parit ; 

}

byte Message::getParit()
{
  //  return this->messageString[MAX_MESSAGE_SIZE-1];
      return this->messageString[this->getMessageLength()+1];
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

