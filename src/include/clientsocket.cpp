#include "clientsocket.hpp"
#include <iostream>

ClientSocket::ClientSocket() {
    /*If we can not create a socket, bind it to a port or listen to incoming connections, throw an error. */
    if (!Socket::create())
        throw SocketException("Cannot create the server socket");
}

const ClientSocket& ClientSocket::operator<< (const std::string& message) const {
    /* If unable to send the message, throw an error. */
    if (!Socket::send(message))
        throw SocketException("Error sending message.");
    return *this;
}

const ClientSocket& ClientSocket::operator>> (std::string& message) {
    /* If unable to receive this message, throw an error. */
    int bytes_read = Socket::recv(message);
    if (bytes_read == -1)
        throw SocketException("Error reading incoming message.");
    return *this;
}

void ClientSocket::connect(const std::string host, const int port) {
    if (!Socket::connect(host, port))
        throw SocketException("Can not connect to the server at " + host + 
                              " port " + std::to_string(port));
}