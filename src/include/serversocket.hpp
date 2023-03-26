#ifndef SERVER_SOCKET__
#define SERVER_SOCKET__

#include "socket.hpp"
#include "socketexception.hpp"
#include <string>

class ServerSocket: private Socket {
public:
    ServerSocket(int port);
    ServerSocket();
    ~ServerSocket() { ServerSocket::close(); };

    const ServerSocket& operator<< (const std::string& message) const; // For sending messages.
    const ServerSocket& operator>> (std::string& message); // For receiving messages.

    void accept(ServerSocket& new_socket) const;
    void close();
};
#endif