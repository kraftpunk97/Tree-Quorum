#include "socket.hpp"

Socket::Socket() {
    m_file_desc = -1;
    memset(&m_adddress, 0, sizeof(m_address));
}

Socket::~Socket() {
    if is_valid()
        ::close(m_file_desc);
}

bool Socket::is_valid() const {return m_file_desc != -1;}


/* Server Initialization functions */

bool Socket::create() {
    /* Sets the file descriptor of the socket and sets the options. */
    m_file_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (!is_valid())
        return false;

    /* TODO: Let's play with it later...*/
    int on = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) == -1)
        return false;

    return true;
}


bool Socket::bind(const int port) {
    /* Configure the m_address to bind a socket to a port*/
    if (!is_valid())
        return false;

    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(port);
    if (::bind(m_file_desc, (const sockaddr*) &m_address, sizeof(m_address)) == -1)
        return false;
    
    return true;
}

bool Socket::listen(int backlog=5) const {
    /* If the socket is valid, then listen for incoming connections. */
    if (!is_valid())
        return false;
    
    if (::listen(m_file_desc, backlog) == -1)
        return false;
    
    return true;
}

bool Socket::accept(Socket& new_socket) const {
    /* Accept the incoming connection. */
    int addr_len = sizeof(m_address);
    new_socket.m_sock = ::accept(m_file_desc, (sockaddr *) &m_address, (socklen_t *) &addr_len);

    if (new_socket == -1)
        return false;
    return true;
}

/* Client Initialization functions */

bool  Socket::connect(const std::string host, const int port) {
    /* Connect to the server at `host` and `port`. */
    if (!is_valid()) {
        return false;
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (::inet_pton(AF_INET, host.c_str(), &server_address.sin_addr) == -1)
        return false;
    return true;
}

/* Data Transmission functions */

bool Socket::send(const std::string data) const {
    /* Send the `data` to established connection. */
    int bytes_sent = ::send(m_file_desc, data.c_str(), data.size(), MSG_NOSIGNAL);
    return bytes_sent != -1;
}

int Socket::recv(std::string& data) {
    /* Recieve the data from the sender. */
    char read_buffer[MAXREADSIZE+1];
    data = "";
    int bytes_read = ::recv(m_file_desc, read_buffer, MAXREADSIZE, 0);
    data = read_buffer;
    return bytes_read;
}