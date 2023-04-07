#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include "include/serversocket.hpp"

int main(int argc, char** argv) {
    const int num_clients = (argc > 1) ? std::atoi(argv[1]) : 5;
    const int port = 8077;
    std::cout << "running server at port " << port << std::endl;

    ServerSocket server_socket(port);
    ServerSocket clients[num_clients];
    int i;
    for(i=0; i<num_clients; i++) {
        server_socket.accept(clients[i]);
        std::cout << "Found a connection." << std::endl;
    }
    std::cout << "Connected to all clients."<< std::endl;
    auto acceptUtil = [](ServerSocket socket) {
        std::string data;
        do {
            socket >> data;
        }while(data.size() < 2);
        socket.close();
    };
    std::thread completion_th[num_clients];
    for (int i=0; i<num_clients; i++) {
        completion_th[i] = std::thread(acceptUtil, clients[i]);
    }
    for (int i=0; i<num_clients; i++) {
        completion_th[i].join();
    }

    std::cout << "Complete." << std::endl;
    return 0;
}