#ifndef CLIENT_CLASS__
#define CLIENT_CLASS__

#include "clientsocket.hpp"
#include <poll.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <set>
#include <iostream>
#include <string>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <unistd.h>
#include <random>

#define MAXSERVERS 7

enum message_t {REQUEST, RELEASE, EXIT};

struct Message {
    message_t message;
    int id;
    std::chrono::high_resolution_clock::time_point timestamp;
};

class Client {
public:
    Client(int designation, int timeunit, std::set <uint> quorums);
    void Handshake(const std::string* host_arr, const int* port_arr, const int num_servers);
    void close();
    void DisplayStatistics();
    void RunClient();

private:
    ClientSocket m_clientsocket[MAXSERVERS];
    ClientSocket m_s0;
    int m_timeunit;
    int m_designation;
    int m_num_servers;
    int m_messages_recieved;
    int m_messages_sent;
    std::set <uint> m_quorums;
};

#endif