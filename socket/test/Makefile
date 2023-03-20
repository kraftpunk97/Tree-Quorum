# Makefile for the socket programming example
#

CXX = g++
CXXFLAGS = -std=c++11

simple_server_objects = ../server/ServerSocket.o ../socket/Socket.o servertest.cpp
simple_client_objects = ../client/ClientSocket.o ../socket/Socket.o clienttest.cpp

all: simple_server simple_client

simple_server: $(simple_server_objects)
	$(CXX) $(CXXFLAGS) $(simple_server_objects) $(simple_server_cpp) -o simple_server

simple_client: $(simple_client_objects)
	$(CXX) $(CXXFLAGS) $(simple_client_objects) -o simple_client

clean:
	rm -rf *.o simple_server simple_client