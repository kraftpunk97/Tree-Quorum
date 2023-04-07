# Makefile
#

base_socket:= include/socket.cpp
main_files:= main_server.cpp main_client.cpp main_server0.cpp
socket_impl := include/serversocket.cpp include/clientsocket.cpp


all : server client server0

server : include/socket.cpp include/serversocket.cpp include/server.cpp main_server.cpp
	g++ include/server.cpp include/serversocket.cpp include/socket.cpp main_server.cpp -o server -std=c++11 -pthread

server0: include/socket.cpp include/serversocket.cpp include/server.cpp main_server0.cpp
	g++ include/serversocket.cpp include/socket.cpp main_server0.cpp -o server0 -std=c++11 -pthread

client : include/socket.cpp include/clientsocket.cpp include/client.cpp main_client.cpp
	g++ include/client.cpp include/clientsocket.cpp include/socket.cpp main_client.cpp -o client -std=c++11 -pthread