#include <iostream>
#include <string>
#include <array>
#include "include/server.hpp"


int main(int argc, char** argv) {
    int port = 8077;
    Server s1(port, 1);
    std::cout << "Server S1 is now running on port " << port << ".\n";
    s1.Handshake();
    return 0;
}