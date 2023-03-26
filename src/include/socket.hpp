#ifndef SOCKET_CLASS__
#define SOCKET_CLASS__

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>


#define MAXREADSIZE 100

class Socket {
private:
    /* A socket is supposed to have an address and a file descriptor */
    int m_file_desc;
    sockaddr_in m_address; 

public:
    Socket();
    //~Socket();
    bool is_valid() const;

    // Server Initialization
    bool create();
    bool bind(const int port);
    bool listen(int backlog=5) const;
    bool accept(Socket& new_socket) const;
    bool close();

    // Client initialization
    bool connect(const std::string host, const int port);

    // Data Transmission
    bool send(const std::string data) const;
    int recv(std::string& data);   
};

#endif