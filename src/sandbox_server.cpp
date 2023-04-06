#include <iostream>
#include <string>
#include <array>
#include <cstring>
#include <cstdlib>
#include <thread>
#include "include/server.hpp"


int main(int argc, char** argv) {
    int designation = argc > 1 ? std::atoi(argv[1]) : 0;
    int port = argc > 2 ? std::atoi(argv[2]) : 8077;
    Server s(port, designation);
    std::cout << "Server S" << designation << " is now running on port " << port << ".\n";
    s.Handshake();
    s.RunServer();
    std::cout << "Shutting down..." << std::endl;
    s.close();
    s.DisplayStatistics();
    return 0;
}