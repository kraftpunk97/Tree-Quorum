#ifndef SERVER_CLASS__
#define SERVER_CLASS__

#include "serversocket.hpp"
#include <iostream>
#include <thread>
#include <ctime>
#include <string>
#include <cstring>
#include <mutex>

#define MAXCLIENTS 2


enum message_t {REQUEST, RELEASE, EXIT};


struct Message {
    message_t message;
    int id;
    std::time_t timestamp;
};

struct MessageNode {
    Message data;
    MessageNode* next;
};

class Server {
public:
    Server(int port, int designation);
    void Handshake();
    void EnqRequest(Message request);
    void listening(int socket_id);
    void DeqRequest(Message request);
    void RunServer();
    void close();

private:
    bool m_state;
    ServerSocket m_serversocket;
    ServerSocket m_clientsockets[MAXCLIENTS];
    MessageNode* m_request_Q;
    int m_designation;
    int m_locked_by;
};

#endif