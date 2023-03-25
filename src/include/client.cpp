# include "client.hpp"

Client::Client(): m_clientsocket() {}; 

void Client::Handshake(const std::string* host_arr, const int* port_arr, const int num_servers) {
    for (auto i=0; i<num_servers; i++) {
        m_clientsocket[i].connect(host_arr[i], port_arr[i]);
        std::cout << "Connected to Server " << i+1 << ".\n";
    }
    std::cout << "Handshakes established.\n";
}