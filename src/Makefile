# Makefile
#

base_socket:= include/socket.cpp
sandbox_files:= sandbox_server.cpp sandbox_client.cpp
socket_impl := include/serversocket.cpp include/clientsocket.cpp


all : server client

server : include/socket.cpp include/serversocket.cpp include/server.cpp sandbox_server.cpp
	g++ include/server.cpp include/serversocket.cpp include/socket.cpp sandbox_server.cpp -o server -std=c++11 -pthread
	g++ include/serversocket.cpp include/socket.cpp s0.cpp -o server0 -std=c++11 -pthread


client : include/socket.cpp include/clientsocket.cpp include/client.cpp sandbox_server.cpp
	g++ include/client.cpp include/clientsocket.cpp include/socket.cpp sandbox_client.cpp -o client -std=c++11 -pthread