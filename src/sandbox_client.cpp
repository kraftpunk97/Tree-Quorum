#include <iostream>
#include "include/client.hpp"
#include <string>
#include <thread>


std::string host_arr[] = { "10.176.69.32",
                           "10.176.69.33"  };
int port_arr[] = {8077, 8077};
const int num_servers = 2;

int main(int argc, char** argv) {
    Client C;
    C.Handshake(host_arr, port_arr, num_servers);

    // Send messages to the two servers
    std::thread thread_arr[num_servers];
    for (int i=0; i<num_servers; i++) {
        //deploy thread func
    }
    C.close();
    return 0;
}