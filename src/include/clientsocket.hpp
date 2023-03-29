#ifndef CLIENT_SOCKET__
#define CLIENT_SOCKET__

#include "socket.hpp"
#include "socketexception.hpp"
#include <string>

class ClientSocket: private Socket {
public:
    ClientSocket();

    const ClientSocket& operator<< (const std::string& message) const; // For sending messages
    const ClientSocket& operator>> (std::string& message); // For receiving messages

    void recv(void* buffer, size_t len);
    void send(const void* buffer, size_t len);
    void connect(const std::string host, const int port);
    bool is_valid() {return Socket::is_valid(); };
    void close();

    void set_non_blocking(const bool blocking) { Socket::set_non_blocking(blocking); };
    int get_file_desc() { return Socket::get_file_desc(); };
};

#endif