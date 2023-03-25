#ifndef CLIENT_SOCKET__
#define CLIENT_SOCKET__

#include "socket.hpp"
#include "socketexception.hpp"
#include <string>

class ClientSocket: private Socket {
public:
    ClientSocket();

    const ClientSocket& operator<< (const std::string& message) const; // For sending messages
    const ClientSocket& operator>> (std::string& message); // For receiving messages

    void connect(const std::string host, const int port);
};

#endif