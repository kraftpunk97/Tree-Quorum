#include "client.hpp"

Client::Client(int designation, int timeunit, std::set<uint> quorums) : m_clientsocket()
{
    m_designation = designation;
    m_timeunit = timeunit;
    m_num_servers = 0;
    m_messages_recieved = 0;
    m_messages_sent = 0;
    m_s0 = ClientSocket();
    m_quorums = quorums;
}

void Client::close()
{
    for (int i = 0; i < m_num_servers; i++)
        m_clientsocket[i].close();
}

void Client::DisplayStatistics()
{
    using namespace std;
    cout << endl
         << endl
         << endl;
    cout << "Client C" << m_designation << " received " << m_messages_recieved << " messages." << endl;
    cout << "Client C" << m_designation << " sent " << m_messages_sent << " messages." << endl;
}

void Client::Handshake(const std::string *host_arr, const int *port_arr, const int num_servers)
{
    /*
    First establish connection to all the servers,
    then wait for the handshake signal from all the servers,
    which signifies that all the clients have connected to all the servers,
    and the server is ready for further processing...
    */
    m_num_servers = num_servers;
    for (auto i = 0; i < m_num_servers; i++)
    {
        m_clientsocket[i].connect(host_arr[i], port_arr[i]);
        std::cout << "Connected to Server " << i + 1 << "." << std::endl;
    }
    m_s0.connect(host_arr[m_num_servers], port_arr[m_num_servers]);
    std::cout << "Connected to Server 0." << std::endl;
    std::cout << "Waiting for the handshake signal from all servers.\n";

    std::thread handshake_th_arr[m_num_servers];
    for (int i = 0; i < m_num_servers; i++)
    {
        auto handshake_util = [](ClientSocket clientsocket)
        {
            std::string recieve_data;
            clientsocket >> recieve_data;
        };
        handshake_th_arr[i] = std::thread(handshake_util, m_clientsocket[i]);
    }

    for (int i = 0; i < m_num_servers; i++)
    {
        handshake_th_arr[i].join();
    }
    std::cout << "Handshake established.\n";
}

void Client::RunClient()
{
    /*
    Steps:
    1. Client creates 7 threads.
        a. Thread sends a request to all the servers.
        b. Thread is now listening for GRANTs.
            * It's a non-blocking socket, so it only stops listening
              when it either gets a GRANT, or a quorum is formed.
            * If either of these conditions are satisfied, the threads are joined.
    2. Execute CS
    3. Client creates 7 threads.
        a. Thread sends a RELEASE to all the servers.
    */

    // Setting up the generator for the sleep duration
    const int range_from = 5;
    const int range_to = 10;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(range_from, range_to);

    // Prep work for poll()
    pollfd pfds[m_num_servers];
    for (int i = 0; i < m_num_servers; i++)
    {
        pfds[i].fd = m_clientsocket[i].get_file_desc();
        pfds[i].events = POLLIN;
    }

    // Helper utililty for sending messages
    auto sendMessage_util = [=](ClientSocket socket, message_t message)
    {
        Message msg;
        msg.timestamp = std::chrono::high_resolution_clock::now();
        msg.message = message;
        msg.id = m_designation;
        socket.send((Message *)&msg, sizeof(msg));
        switch (message)
        {
        case REQUEST:
            std::cout << "Request ";
            break;
        case RELEASE:
            std::cout << "Release ";
            break;
        case EXIT:
            std::cout << "Exit ";
            break;
        }
        std::cout << "sent to Server." << std::endl;
    };

    for (int i = 0; i < 20; i++)
    {
        std::cout << "\n\n\nRound " << i + 1 << " begins..." << std::endl;

        // Sleep before the round begins
        uint sleeptime = distr(generator) * m_timeunit;
        std::cout << "Sleeping for " << sleeptime << " microseconds" << std::endl;
        usleep(sleeptime);

        // std::time_t round_start_time = std::time(nullptr);
        auto round_start_time = std::chrono::high_resolution_clock::now();
        // Send the requests
        std::thread request_th[m_num_servers];
        for (int i = 0; i < m_num_servers; i++)
        {
            request_th[i] = std::thread(sendMessage_util, m_clientsocket[i], REQUEST);
            // request_th[i].detach();
        }

        for (int i = 0; i < m_num_servers; i++)
        {
            request_th[i].join();
        }
        m_messages_sent += m_num_servers;

        // While we don't have a quorum, select() the sockets that have data ready to be read.
        uint grantrecvd = 0x00;
        bool quorum_found = false;
        int num_prev_events = 0;
        int num_events;
        int num_grantsrecvd = 0;
        std::string grant_message;
        do
        {
            num_events = poll(pfds, m_num_servers, 4);
            if (num_events > num_prev_events)
            {
                for (int i = 0; i < m_num_servers; i++)
                {
                    if (pfds[i].revents & POLLIN)
                    {
                        /*
                        This means that we have data to read at this socket.
                        We also need to account for the 0-byte datagrams,
                        So we make sure that the message we get has some content to begin with.
                        */
                        m_clientsocket[i] >> grant_message; // TODO: Fix this shit....
                        if (grant_message.size() > 2)
                        {
                            grantrecvd = grantrecvd ^ (1 << i);
                            std::cout << "Recieved Grant from S" << i + 1 << std::endl;
                        }
                    }
                    for (uint quorum : m_quorums)
                    {
                        if ((grantrecvd & quorum) == quorum)
                        {
                            quorum_found = true;
                            std::cout << "Found a quorum " << quorum << std::endl;
                            while (grantrecvd)
                            {
                                num_grantsrecvd += grantrecvd & 1;
                                grantrecvd >>= 1;
                            }
                            break;
                        }
                    }
                    if (quorum_found)
                        break;
                }
            }
            num_prev_events = num_events;
        } while (!quorum_found);

        // Execute critical section
        // std::time_t entering_cs = std::time(nullptr);
        auto entering_cs = std::chrono::high_resolution_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::microseconds>(entering_cs - round_start_time).count();
        // std::cout << "Entering CS: " << m_designation << " at " << std::asctime(std::localtime(&entering_cs));
        std::cout << "Entering CS: " << m_designation << std::endl;
        std::cout << "Messages exchanged: " << (m_num_servers + num_grantsrecvd) << std::endl;
        std::cout << "Latency: " << latency << "usec" << std::endl;
        std::cout << "Critical Section executing for " << 3 * m_timeunit << " microseconds" << std::endl;
        usleep((uint)3 * m_timeunit);

        // send releases()
        std::thread release_th[m_num_servers];
        for (int i = 0; i < m_num_servers; i++)
        {
            release_th[i] = std::thread(sendMessage_util, m_clientsocket[i], RELEASE);
        }
        for (int i = 0; i < m_num_servers; i++)
        {
            release_th[i].join();
        }
        m_messages_sent += m_num_servers;
        m_messages_recieved += num_grantsrecvd;
    }

    std::thread exit_th[m_num_servers];
    for (int i = 0; i < m_num_servers; i++)
    {
        exit_th[i] = std::thread(sendMessage_util, m_clientsocket[i], EXIT);
        // exit_th[i].detach();
    }
    for (int i = 0; i < m_num_servers; i++)
    {
        exit_th[i].join();
    }

    usleep(100000);
    m_s0 << "Computation complete";
    usleep(100000);
    m_s0.close();
}