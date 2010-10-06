all:
	g++ -o cliente cliente.cpp socket.cpp
	g++ -o servidor servidor.cpp socket.cpp
