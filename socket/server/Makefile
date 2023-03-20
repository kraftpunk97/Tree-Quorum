# Makefile for the server socket.
#

CXX = g++
CXXFLAGS = -std=c++11 -c

base_socket_object = Socket.o
server_socket_cpp = serversocket.cpp

all: $(server_socket_cpp)
	$(CXX) $(CXXFLAGS) $(server_socket_cpp) -o ServerSocket.o

clean:
	rm -rf *.o