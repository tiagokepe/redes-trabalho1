#include "message.h"

Message::Message(byte *msg) {
	/* Usado para copiar mensagens na integra. */
    if (msg)
    	cloneMessage(msg);
}

/* Usado para contruir mensagens a partir de dados. */
Message::Message(byte *msg, MessageType mt, int seq) {
    byte * data = ( msg == NULL )?( byte * )strdup(""):msg; 
	
	if ( this->setMessage(data, (size_t)strlen( (const char *)data ), mt, seq) )
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

    cout << "dataSize = " << dataSize << endl;

	if ( setMessageLength(dataSize) == -1 ) return -1;

	setMessageSequence(seq);
    cout << "Type = " << mt << endl;
	setMessageType(mt);
	
    cout << "Setou seq e type" << endl;

	/* Parte de dados do buffer é mapeado para */
    for(i=0; i<dataSize; i++)
        this->messageString[i+4] = data[i];
    
    cout << "Copiou dados" << endl;

    this->generateParit();
    cout << "Saiu no set message" << endl;

	return 0;
}

/* Retorna se esta mensagem é valida. */
bool Message::messageValida(void) {
    cout << "MEssage Valida" << endl;
    if ( (this->messageString[0] == byte(BEGIN_MARKER) ) && ( this->valida ) )
        return true;
    
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
    size_t tam = this->getMessageLength()-2;
    for(i=0; i < tam; i++)
        cout << this->messageString[i+4];
    cout << endl;
//    cout << "Paridade = " << (int ) this->getParit() << endl;
}

int Message::setMessageLength(size_t dataSize )
{
	/* Tamanho da mensagem descarta marcador de início e próprio espaço para tamanho. */ 
	if( dataSize > ( MAX_MESSAGE_SIZE - 2 ) )
		return -1;
    
	this->messageString[1] = (byte) (dataSize +3);

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
    cout << "CLONOU" << endl;
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
    size_t i;
    size_t size = this->getMessageLength()-2;
    cout << "No gerar = " << size << endl;
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
    cout << "Gerou paridade = " << this->messageString[this->getMessageLength()+1] << endl;
}

