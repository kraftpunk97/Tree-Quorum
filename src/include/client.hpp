#ifndef CLIENT_CLASS__
#define CLIENT_CLASS__

#include "clientsocket.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <thread>

#define MAXSERVERS 5

class Client {
public:
    Client();
    void Handshake(const std::string* host_arr, const int* port_arr, const int num_servers);
    //void SendMessage(std::string message, int server_num);
    //void RecvMessage(std::string message, int server_num);
    void close();

private:
    ClientSocket m_clientsocket[MAXSERVERS];
};

#endif