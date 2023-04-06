#include "server.hpp"

std::mutex g_req_Q_mutex;

Server::Server(int port, int designation): m_serversocket(port) {
    m_state = false;
    m_designation = designation;
    m_request_Q = nullptr;
    m_messages_recieved = 0;
    m_messages_sent = 0;
}

void Server::close() {
    m_serversocket.close();
}

void Server::DisplayStatistics() {
    using namespace std;
    cout << endl << endl << endl;
    cout << "Server S" << m_designation << " received " << m_messages_recieved << " messages." << endl;
    cout << "Server S" << m_designation << " sent " << m_messages_sent << " messages." << endl;
}

void printQ(MessageNode* head) {
    int count = 0;
    if (head == nullptr) {
        std::cout << "Empty queue" << std::endl;
    }
    else {
        MessageNode* ctr = head;
        while (ctr != nullptr) {
            if (ctr == head)
                std::cout << "Locked by :" << ctr->data.id << "\tQueue: ";
            else
                std::cout << ctr->data.id;
            ctr = ctr->next;
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
    std::cout << "Listening for messages from C" << socket_id+1 << std::endl;
    while(true) {
        std::cout << "\n";
        Message msg;
        memset(&msg, 0, sizeof(msg));
        try {
            int bytes_read = socket.recv(&msg, sizeof(msg));
            if (bytes_read == 0) {
                continue;
            }
        }
        catch (SocketException&) {
            std::cout << "Connection dropped by C" << socket_id+1 << std::endl;
            return;
        }
    
        // If we get a request, then enqueue it
        if (msg.message == REQUEST) {
            g_req_Q_mutex.lock();
            m_messages_recieved++;
            std::cout << "REQUEST by C" << msg.id << std::endl;
            Server::EnqRequest(msg);
            if (!m_state) {
                m_state = true;
                Message new_lock = m_request_Q->data;
                m_locked_by = new_lock.id;
                m_clientsockets[new_lock.id-1] << "GRANT";
                m_messages_sent++;
                std::cout << "Grant sent to " << new_lock.id << std::endl;  
            }
            g_req_Q_mutex.unlock();
        }
        else if (msg.message == RELEASE) {
            if (m_locked_by == msg.id) {
                // remove element from the queue
                g_req_Q_mutex.lock();
                m_messages_recieved++;
                std::cout << "RELEASE by C" << msg.id << std::endl;
                Server::DeqRequest(msg); // TGus should be the first message
                m_state = m_request_Q != nullptr; // Locked if the queue is not empty
                if (m_state) {
                    // If the server is still locked after the dequeue, send grant to the client at the top of the queue;
                    Message new_lock = m_request_Q->data;
                    m_locked_by = new_lock.id;
                    m_messages_sent++;
                    m_clientsockets[new_lock.id-1] << "GRANT";
                    std::cout << "GRANT sent to " << new_lock.id << std::endl;                                                                              
                }
                g_req_Q_mutex.unlock();
            }
            else {
                g_req_Q_mutex.lock();
                m_messages_recieved++;
                std::cout << "RELEASE by C" << msg.id << std::endl;
                Server::DeqRequest(msg);
                g_req_Q_mutex.unlock();
            }
        }
        else if (msg.message == EXIT) {
            m_clientsockets[socket_id].close();
            std::cout << "EXIT by C" << msg.id << std::endl;
            break;
        }
    }
}

void Server::DeqRequest(Message request) {
    if (m_request_Q != nullptr) { // IF the equest queue is not empty...
        MessageNode* ctr = m_request_Q;
        if (m_request_Q->data.id == request.id) {
            m_request_Q = m_request_Q->next;
        }
        else {
            while ((ctr->next)->data.id != request.id) {
                ctr = ctr->next;
            }
            ctr->next = ctr->next->next;
        }
        //delete ctr;
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
        using namespace std::chrono;
        MessageNode* ctr = m_request_Q;
        while (duration_cast<microseconds>(ctr->data.timestamp - new_ptr->data.timestamp).count() < 0 and 
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
    usleep(100000);
}