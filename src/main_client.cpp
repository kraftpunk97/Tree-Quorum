#include <iostream>
#include "include/client.hpp"
#include "include/clientsocket.hpp"
#include <string>
#include <thread>
#include <set>

#define TIMEUNIT 500000 // This is in microseconds
std::string host_arr[] = {
    "10.176.69.32",
    "10.176.69.33",
    "10.176.69.34",
    "10.176.69.35",
    "10.176.69.36",
    "10.176.69.37",
    "10.176.69.38",
    "10.176.69.39",
};
int port_arr[] = {8077, 8077, 8077, 8077, 8077, 8077, 8077, 8077};
const int num_servers = 7;

int main(int argc, char **argv)
{
    int designation = argc > 1 ? std::atoi(argv[1]) : 0;

    std::set<uint> quorums = {11, 19, 25,
                              37, 69, 97,
                              46, 78, 106,
                              54, 86, 114,
                              60, 92, 120};
    Client C(designation, TIMEUNIT, quorums);
    C.Handshake(host_arr, port_arr, num_servers);
    C.RunClient();
    std::cout << "Now closing client..." << std::endl;
    C.close();
    C.DisplayStatistics();

    return 0;
}