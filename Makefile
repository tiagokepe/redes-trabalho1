CC = g++

all: msg sock server client 
	$(CC) -o cliente cliente.o socket.o message.o
	$(CC) -o servidor servidor.o socket.o message.o

server: servidor.h
	$(CC) -o cliente.o -c cliente.cpp

client: cliente.h
	$(CC) -o servidor.o -c servidor.cpp

sock: socket.h
	$(CC) -o socket.o -c socket.cpp

msg: message.h
	$(CC) -o message.o -c message.cpp
clean:
	rm -rf cliente servidor 
	rm -rf *.o *~
