#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

#include "parser.hpp"
#include "threadpool.hpp"

using namespace std;
#define PORT "12345"

class Server
{
private:
    threadpool pool;
    XMLParser parser;
    Database db;
    int server_sockfd;
    int status;
    struct addrinfo host;
    struct addrinfo *host_list;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

public:
    Server();
    void run();
    void process(int new_fd);
    string recv_request(int new_fd);

    ~Server();
};

#endif // SERVER_HPP