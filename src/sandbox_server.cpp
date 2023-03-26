#include <iostream>
#include <string>
#include <array>
#include <cstring>
#include "include/server.hpp"


int main(int argc, char** argv) {
    int designation = argc > 1 ? std::atoi(argv[1]) : 0;
    int port = argc > 2 ? std::atoi(argv[2]) : 8077;
    Server s(port, designation);
    std::cout << "Server S" << designation << " is now running on port " << port << ".\n";
    s.Handshake();
    s.close();
    return 0;
}