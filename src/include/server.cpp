#include "server.hpp"

Server::Server(int port, int designation): m_serversocket(port) {
    m_designation = designation;
}

void Server::Handshake() {
    std::cout << "Waiting for the clients to connect...\n";
    for (int i=0; i<MAXCLIENTS; i++) {
        ServerSocket new_client;
        m_serversocket.accept(new_client);
        std::cout << "Found a connection...\n";
        m_clientsockets[i] = new_client;
    }
    std::cout << "\nHandshake established...\n";
}
