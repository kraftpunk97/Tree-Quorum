#include <iostream>
#include "include/client.hpp"
#include <string>


std::string host_arr[] = { "10.176.69.32",
                           "10.176.69.33"  };
int port_arr[] = {8077, 8077};
int num_servers = 2;

int main(int argc, char** argv) {
    Client C1;
    C1.Handshake(host_arr, port_arr, num_servers);
    return 0;
}