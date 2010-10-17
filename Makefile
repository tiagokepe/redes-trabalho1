CC = g++

all: msg sock control server client 
	$(CC) -o cliente cliente.o control.o socket.o message.o
	$(CC) -o servidor servidor.o control.o socket.o message.o

server: servidor.h
	$(CC) -o cliente.o -c cliente.cpp

client: cliente.h
	$(CC) -o servidor.o -c servidor.cpp

control: control.h
	$(CC) -o control.o -c control.cpp

sock: socket.h
	$(CC) -o socket.o -c socket.cpp

msg: message.h
	$(CC) -o message.o -c message.cpp
clean:
	rm -rf cliente servidor 
	rm -rf *.o *~
