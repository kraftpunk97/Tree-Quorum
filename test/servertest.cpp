#include "../src/include/serversocket.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    int port = (argc > 1) ? std::atoi(argv[1]) : 8077;

    std::cout << "running server at port " << port << "...\n";
    try {
        ServerSocket server_socket(8077);
        std::cout << "line 1\n";
        while (true) {
            ServerSocket client_socket;
            std::cout << "line 2\n";
            server_socket.accept(client_socket);
            std::cout << "line 3\n";
            std::cout << "found a connection\n";
            try {
                std::string data;
                client_socket >> data;
                client_socket << data;
            } catch (SocketException&) {}
        }
    } catch (SocketException& e) {
        std::cerr << "something went wrong...\n"; 
        return 1;
    }

    return 0;
}
