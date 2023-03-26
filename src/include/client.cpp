# include "client.hpp"

Client::Client(): m_clientsocket() {};

void Client::close() {
    for (int i=0; i<2; i++)
        m_clientsocket[i].close();
}

void Client::Handshake(const std::string* host_arr, const int* port_arr, const int num_servers) {
    /*
    First establish connection to all the servers, 
    then wait for the handshake signal from all the servers,
    which signifies that all the clients have connected to all the servers,
    and the server is ready for further processing...
    */
    for (auto i=0; i<num_servers; i++) {
        m_clientsocket[i].connect(host_arr[i], port_arr[i]);
        std::cout << "Connected to Server " << i+1 << ".\n";
    }
    std::cout << "Waiting for the handshake signal from all servers.\n";
    
    std::thread handshake_th_arr[num_servers];
    for (int i=0; i<num_servers; i++) {
        auto handshake_util = [](ClientSocket clientsocket) {
            std::string recieve_data;
            while (recieve_data.empty()) {
                clientsocket >> recieve_data;
            }
            std::cout << recieve_data << std::endl;
        };
        handshake_th_arr[i] = std::thread(handshake_util, m_clientsocket[i]);
    }

    for (int i=0; i<num_servers; i++) {
        handshake_th_arr[i].join();
    }
    std::cout << "Handshake established.\n";
}