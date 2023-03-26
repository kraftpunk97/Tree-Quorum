#include "server.hpp"

Server::Server(int port, int designation): m_serversocket(port) {
    m_designation = designation;
}

void Server::close() {
    for (int i=0; i<MAXCLIENTS; i++)
        m_clientsockets[i].close();
    m_serversocket.close();
}

void Server::Handshake() {
    std::thread handshake_th_arr[MAXCLIENTS];
    std::cout << "Waiting for the clients to connect...\n";
    for (int i=0; i<MAXCLIENTS; i++) {
        ServerSocket new_client;
        m_serversocket.accept(new_client);
        std::cout << "Found a connection...\n";
        m_clientsockets[i] = new_client;
    }
    std::cout << "\nAll clients connected\n";
    std::cout << "\nSending handshake signal to all clients\n";
    for (int i=0; i<MAXCLIENTS; i++) {
        auto handshake_util = [](ServerSocket socket) {
            socket << "Handshake";
            std::cout << "Handshake signal sent" << std::endl;
        };
        handshake_th_arr[i] = std::thread(handshake_util, m_clientsockets[i]);
    }
    for (int i=0; i<MAXCLIENTS; i++) {
        handshake_th_arr[i].join();
    }
    std::cout << "\nHandshake established...\n";
}


