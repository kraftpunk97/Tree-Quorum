#ifndef SERVER_CLASS__
#define SERVER_CLASS__

#include "serversocket.hpp"
#include <iostream>
#include <vector>

#define MAXCLIENTS 1

class Server {
public:
    Server(int port, int designation);
    //TODO: Maybe create destructor
    void Handshake();
    void EnqRequest(int request);
    void DeqRequest(int request);

private:
    ServerSocket m_serversocket;
    ServerSocket m_clientsockets[MAXCLIENTS];
    int m_designation;
    std::vector<int> m_request_Q[MAXCLIENTS];
};

#endif