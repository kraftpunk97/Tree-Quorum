# Makefile for the client socket.
#

CXX = g++
CXXFLAGS = -std=c++11 -c

base_socket_object = Socket.o
client_socket_cpp = clientsocket.cpp

all: $(client_socket_cpp)
	$(CXX) $(CXXFLAGS) $(client_socket_cpp) -o ClientSocket.o

clean:
	rm -rf *.o