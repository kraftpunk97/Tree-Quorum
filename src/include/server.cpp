#include "server.hpp"

std::mutex g_req_Q_mutex;

Server::Server(int port, int designation): m_serversocket(port) {
    m_state = false;
    m_designation = designation;
    m_request_Q = nullptr;
}

void Server::close() {
    for (int i=0; i<MAXCLIENTS; i++)
        m_clientsockets[i].close();
    m_serversocket.close();
}

void printQ(MessageNode* head) {
    int count = 0;
    if (head == nullptr) {
        std::cout << "Empty queue" << std::endl;
    }
    else {
        MessageNode* ctr = head;
        while (ctr != nullptr) {
            std::cout << ctr->data.id;
            ctr = ctr->next;
            count++;
        }
        std::cout << std::endl;
    }
}

void Server::Handshake() {
    std::thread handshake_th_arr[MAXCLIENTS];
    std::cout << "Waiting for the clients to connect...\n";
    for (int i=0; i<MAXCLIENTS; i++) {
        ServerSocket new_client;
        m_serversocket.accept(new_client);
        std::cout << "Found a connection...\n";
        m_clientsockets[i] = new_client;
    }
    std::cout << "\nAll clients connected\n";
    std::cout << "\nSending handshake signal to all clients\n";
    for (int i=0; i<MAXCLIENTS; i++) {
        auto handshake_util = [](ServerSocket socket) {
            socket << "Handshake";
            std::cout << "Handshake signal sent" << std::endl;
        };
        handshake_th_arr[i] = std::thread(handshake_util, m_clientsockets[i]);
    }
    for (int i=0; i<MAXCLIENTS; i++) {
        handshake_th_arr[i].join();
    }
    std::cout << "\nHandshake established...\n";
}

void Server::listening(int socket_id) {
    ServerSocket socket = m_clientsockets[socket_id];
    while(true) {
        //std::cout << "Listening for message\n";
        Message msg;
        memset(&msg, 0, sizeof(msg));
        int bytes_read = socket.recv(&msg, sizeof(msg));
        if (bytes_read == 0) {
            std::cout << "Connection closed by client " << socket_id+1 << "\n";
            return;
        }
        //std::cout <<  "Recieved a message " << msg.message << " by " << msg.id << " at "  << std::asctime(std::localtime(&msg.timestamp)) << std::endl;
    
        // If we get a request, then enqueue it
        if (msg.message == REQUEST) {
            g_req_Q_mutex.lock();
            Server::EnqRequest(msg);
            g_req_Q_mutex.unlock();
        }
        else if (msg.message == RELEASE) {
            if (m_locked_by == socket_id) {
                // remove element from the queue
                g_req_Q_mutex.lock();
                Server::DeqRequest(msg);
                m_state = m_request_Q != nullptr; // Locked if the queue is not empty
                if (m_state) {
                    // If the server is still locked after the dequeue, send grant to the client at the top of the queue;
                    Message new_lock = m_request_Q->data;
                    m_locked_by = new_lock.id;
                    m_clientsockets[new_lock.id] << "GRANT";                                                                              
                }
                g_req_Q_mutex.unlock();
            }
            else {
                g_req_Q_mutex.lock();
                Server::DeqRequest(msg);
                g_req_Q_mutex.unlock();
            }
        }
        else if (msg.message == EXIT) {
            break;
        }
    }
}

void Server::DeqRequest(Message request) {
    if (m_request_Q != nullptr) {
        MessageNode* ctr = m_request_Q;
        if (m_request_Q->data.id == request.id) {
            m_request_Q = ctr->next;
        }
        else {
            while (ctr->next->data.id != request.id) {
                ctr = ctr->next;
            }
            ctr->next = ctr->next->next;
        }
    }
    printQ(Server::m_request_Q);
}


void Server::EnqRequest(Message msg) {
    MessageNode* new_ptr = new MessageNode;
    new_ptr->data = msg;
    if (m_request_Q == nullptr) {
        new_ptr->next = nullptr;
        m_request_Q = new_ptr;
    }
    else {
        MessageNode* ctr = m_request_Q;
        while (std::difftime(ctr->data.timestamp, new_ptr->data.timestamp) < 0 and 
               ctr->next != nullptr)
            ctr = ctr->next;
        new_ptr->next = ctr->next;
        ctr->next = new_ptr;
    }
    printQ(Server::m_request_Q);
}

void Server::RunServer() {
    int request_Q[MAXCLIENTS];
    std::thread request_release_wait[MAXCLIENTS];
    
    for (int i=0; i<MAXCLIENTS; i++) {
        request_release_wait[i] = std::thread(&Server::listening, this, i);
    }

    for (int i=0; i<MAXCLIENTS; i++) {
        request_release_wait[i].join();
    }
}