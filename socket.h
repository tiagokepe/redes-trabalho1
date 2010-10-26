#ifndef H_SOCKET_H
#define H_SOCKET_H

#include <sys/socket.h>
#include <iostream>
#include <sys/ioctl.h>
#include <net/if.h>

#include <net/ethernet.h>
#include <linux/if_packet.h>
//#include <netinet/in.h>

#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "message.h"

#define DEVICE "eth1"
class RawSocket {
	public:
		RawSocket(void);
		bool socketValido() const { return socket_desc != -1; }
        void sendMessage(Message *msg);
        Message *getMessage(void);
        int getDescriptor(void);
		void cleanBuf(byte *buffer, int size);
    private:
		bool server;
		int socket_desc;
		sockaddr_ll socket_endereco;
		bool modoPromiscuo(struct ifreq);
		bool atribuirEndereco(char *);
		bool criar();
		Message *cloneMessage(byte *data);
		Message *receiveMessage;


};

#endif

