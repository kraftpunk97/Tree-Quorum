#ifndef SERVER_SOCKET__
#define SERVER_SOCKET__

#include "../socket/socket.hpp"
#include "../socket/socketexception.hpp"
#include <string>

class ServerSocket: private Socket {
public:
    ServerSocket(int port);
    ServerSocket();
    virtual ~ServerSocket();

    const ServerSocket& operator<< (const std::string& message) const; // For sending messages.
    const ServerSocket& operator>> (std::string& message); // For receiving messages.

    void accept(ServerSocket& new_socket) const;

};
#endif