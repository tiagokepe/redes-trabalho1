all:
	g++ -o cliente Cliente.cpp ClienteSocket.cpp
	g++ -o servidor servidor.cpp ServidorSocket.cpp
