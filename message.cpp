#include "message.h"

Message::Message(byte *msg) {
	if ( msg )  
		cloneMessage(msg);

}

/* Usado para contruir mensagens a partir de dados. */
Message::Message(byte *msg, MessageType mt, int seq) {
    byte * data = ( msg == NULL )?( byte * )strdup(""):msg; 
	

	if ( this->setMessage(data, (size_t) ( strlen( (const char *)data) + 1 ),mt, seq) )
	{
		this->valida = false;
		cerr << "Esta mensagem é inválida" << endl;
	}
	else
		this->valida = true;
}



int Message::setMessage(byte *data, size_t dataSize, MessageType mt, int seq) {
    cout << "Entrou no set message" << endl;
    this->messageString[0] = byte(BEGIN_MARKER);
    size_t i;

//	if( msg == NULL ) dataSize = 0;

	if ( setMessageLength(dataSize) == -1 ) return -1;

	setMessageSequence(seq);
    cout << "Type = " << mt << endl;
	setMessageType(mt);
	
    cout << "Setou seq e type" << endl;

	/* Parte de dados do buffer é mapeado para */
    for(i=0; i<dataSize; i++)

        this->messageString[i+4] = data[i];

    this->generateParit();


	return 0;
}

/* Retorna se esta mensagem é valida. */
bool Message::messageValida(void) {

    if ( !( (this->messageString[0] == byte(BEGIN_MARKER) ) && ( this->valida ) && ( checkParity() ) ) )
		return false;

	return true;
}

bool Message::checkParity(void) {


    size_t i;
    size_t size = this->getMessageLength()-2;
    byte parit[2];
    parit[0] = parit[1] = 0;

    /* Checa paridade de 16 bits */
    for(i=0; i < size; i++)

       if((i%2) == 0)
            parit[0] ^= this->messageString[i+3];
        else
            parit[1] ^= this->messageString[i+3];

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
    size_t tam = this->getMessageLength()-3;
    for(i=0; i < tam; i++)
        cout << this->messageString[i+4];
    cout << "*" << endl;
    cout << "Paridade = " << this->getParit() << endl;
}

int Message::setMessageLength(size_t dataSize )
{
	/* Tamanho máximo da string = Tamanho de dados - Outros campos */ 
	if( dataSize > ( MAX_MESSAGE_SIZE - 5 ) )
		return -1;

	dataSize +=3;
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

void Message::cloneMessage(byte *msg)
{
	size_t i;
    size_t msgSize = (size_t)(msg[1])+2;
    cout << "No clone tam = " << msgSize << endl;
	this->valida = true;
	for (i= 0; i < msgSize; i++)
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
    size_t size = this->getMessageLength()-2;
    byte parit[2];
    parit[0] = parit[1] = 0;

    /* Gera paridade de 16 bits */
    for(i=0; i < size; i++)
        if((i%2) == 0)
            parit[0] ^= this->messageString[i+3];
        else
            parit[1] ^= this->messageString[i+3];

    /* Gera paridade em 8 bits */
    parit[0] ^= parit[1];
    cout << "Parit = " << parit[0] << endl;
    this->setParit(parit[0]);

}

