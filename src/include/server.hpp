#ifndef SERVER_CLASS__
#define SERVER_CLASS__

#include "serversocket.hpp"
#include <iostream>
#include <vector>
#include <thread>

#define MAXCLIENTS 2

class Server {
public:
    Server(int port, int designation);
    //TODO: Maybe create destructor
    void Handshake();
    //void EnqRequest(int request);
    //void DeqRequest(int request);
    //void SendMessage(std::string message, int client_num) const;
    //void RecvMessage(std::string message, int client_num) const;
    
    void close();

private:
    ServerSocket m_serversocket;
    ServerSocket m_clientsockets[MAXCLIENTS];
    int m_designation;
    std::vector<int> m_request_Q[MAXCLIENTS];
};

#endif