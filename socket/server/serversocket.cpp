#include "serversocket.hpp"
#include <iostream>

ServerSocket::ServerSocket(int port) {
    /*If we can not create a socket, bind it to a port or listen to incoming connections, throw an error. */
    if (!Socket::create())
        throw SocketException("Cannot create the server socket");
    
    if (!Socket::bind(port))
        throw SocketException("Cannot bind the server to port " + std::to_string(port));
    
    if (!Socket::listen())
        throw SocketException("Error listening for incoming connection requests at port " + std::to_string(port));
}

ServerSocket::ServerSocket() {}

ServerSocket::~ServerSocket() {}


const ServerSocket& ServerSocket::operator<< (const std::string& message) const {
    /* If unable to send the message, throw an error. */
    if (!Socket::send(message))
        throw SocketException("Error sending message.");
    return *this;
}

const ServerSocket& ServerSocket::operator>> (std::string& message) {
    /* If unable to receive this message, throw an error. */
    int bytes_read = Socket::recv(message);
    if (bytes_read == -1)
        throw SocketException("Error reading incoming message.");
    return *this;
}


void ServerSocket::accept(ServerSocket& new_socket) const {
    /* If we cannot accept this exception, throw an error*/
    if(!Socket::accept(new_socket))
        throw SocketException("Cannot accept the incoming connection.");
    std::cout << "Socket has found a connection.";
}