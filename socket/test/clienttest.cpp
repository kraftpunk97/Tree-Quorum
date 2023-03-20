#include "../client/clientsocket.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {

    
    if(argc < 3) {
        std::cerr << "missing arguments. enter server ip and port.\n";
        return 1;
    }
    std::string server_address = argv[1];
    int port = std::atoi(argv[2]);

    std::cout << "running client...\n";
    try {
        ClientSocket client_socket;
        client_socket.connect(server_address, port);
        std::cout << "found the server.\n";
        
        std::string transmit_data = "The quick brown fox jumped over the lazy dog.";
        std::string recieve_data;

        client_socket << transmit_data;
        std::cout << "Successfully transmitted the message.\n";

        client_socket >> recieve_data;
        std::cout << "Successfully recieved the message: " << recieve_data << std::endl;
        
    } catch(SocketException& e) {
        std::cerr << "something's not right...\n";
        std::cerr << e.what();

        return 1;
    }
    return 0;
}