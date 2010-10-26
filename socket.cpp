#include "socket.h"
using namespace std;

RawSocket::RawSocket(void) {
    if (!criar()  || !atribuirEndereco((char *) DEVICE ) ) exit(-1);
}

bool RawSocket::criar() {
    this->socket_desc = socket(AF_PACKET, SOCK_RAW, 0);
    if ( !socketValido() )
        return false;
    return true;
}

bool RawSocket::atribuirEndereco(char *placa) {

	struct ifreq descricao_interface;

	if ( !socketValido() )
		return false;

	memset(&descricao_interface,0,sizeof(descricao_interface));
	/* Copia nome da placa alvo para a estrutura de identificação. */
	strcpy(descricao_interface.ifr_name,placa);

	/* Retorna mapeamento da placa. */
	if ( ioctl(this->socket_desc,SIOCGIFINDEX,&descricao_interface) != 0 )
		return false;

	/* Informações sobre a interface. */
	this->socket_endereco.sll_family = AF_PACKET;
	this->socket_endereco.sll_ifindex = descricao_interface.ifr_ifindex;
	this->socket_endereco.sll_protocol = htons(ETH_P_ALL);

	/* Atribui endereco da placa alvo. */
	if ( bind(this->socket_desc, (struct sockaddr *) &this->socket_endereco, sizeof(this->socket_endereco)) )
		return false;

	modoPromiscuo(descricao_interface);

	return true;
}

bool RawSocket::modoPromiscuo(struct ifreq desc)
{
	packet_mreq mr;

	memset(&mr,0,sizeof(mr));

	mr.mr_ifindex = desc.ifr_ifindex;
	mr.mr_type = PACKET_MR_PROMISC;

	if( setsockopt(this->socket_desc,SOL_PACKET,PACKET_ADD_MEMBERSHIP,&mr,sizeof(mr)) )
		return false;

	return true;
}

int RawSocket::getDescriptor(void)
{
    return this->socket_desc;
}

void RawSocket::sendMessage(Message *msg) {
    send(this->getDescriptor(), msg->getMessageString(), MAX_MESSAGE_SIZE, 0);
}

Message *RawSocket::getMessage(void) {
    Message *msg = NULL;
	byte buffer[MAX_MESSAGE_SIZE];
	cleanBuf(buffer,MAX_MESSAGE_SIZE);
    if( recv(this->getDescriptor(), buffer, MAX_MESSAGE_SIZE, 0) == -1)
    {
//        printf("Error: ao receber\n");
        return NULL;
    }
	/* Mensagem recebida é copiada localmente. */
    msg = new Message(buffer);

//    cout << "Tipo = " << msg->getMessageType() << endl;

    return msg;
}

/* Buffer deve ser limpado entre envios para evitar que lixo seja enviado. */
void RawSocket::cleanBuf(byte *buffer, int size)
{
	for(int i =0; i < size; i++)
		*( buffer + i ) = '\0';
}

