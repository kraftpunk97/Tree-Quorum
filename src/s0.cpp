#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include "include/serversocket.hpp"

int main(int argc, char** argv) {
    std::printf("Hello");
    const int num_clients = (argc > 1) ? std::atoi(argv[1]) : 5;
    std::cout << "running server at port ";

    ServerSocket server_socket(8077);
    ServerSocket clients[num_clients];
    for(int i=0; i<num_clients; i++) {
        server_socket.accept(clients[i]);
    }
    std::cout << "Connected to all clients" << std::endl;
    auto acceptUtil = [](ServerSocket socket) {
        std::string data;
        do {
            socket >> data;
        }while(data.size() > 2);
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