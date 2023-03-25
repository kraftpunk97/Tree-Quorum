#ifndef CLIENT_CLASS__
#define CLIENT_CLASS__

#include "clientsocket.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <string>

#define MAXSERVERS 5

/* struct Address {
    std::string host;
    int port;
    Address(std::string host, int port)
        :host(host), port(port) {}
    Address() {}
};*/

class Client {
public:
    Client();
    void Handshake(const std::string* host_arr, const int* port_arr, const int num_servers);
private:
    ClientSocket m_clientsocket[MAXSERVERS];
};

#endif