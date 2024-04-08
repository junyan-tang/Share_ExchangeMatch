#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <time.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT "12345"
#define BUFF_SIZE 10240
#define MAX_THREAD 2

int createSocketAndConnect(const char* serverAddr, const char* serverPort) {
    struct addrinfo hints, *res;
    int serverSfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(serverAddr, serverPort, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    serverSfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (serverSfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (connect(serverSfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res); 
    return serverSfd;
}

void sendXmlRequest(int sockfd, const std::string& filePath) {
    std::ifstream fs(filePath);
    std::stringstream buffer;
    buffer << fs.rdbuf(); 
    std::string req = buffer.str();

    std::string prefixedReq = std::to_string(req.length()) + "\n" + req; 
    send(sockfd, prefixedReq.c_str(), prefixedReq.length(), 0); 
}

std::string receiveResponse(int sockfd) {
    char buffer[BUFF_SIZE];
    memset(buffer, 0, sizeof(buffer)); 
    recv(sockfd, buffer, sizeof(buffer), 0); 
    return std::string(buffer);
}

void* handler(void* arg) {
    char* filePath = static_cast<char*>(arg);
    int sockfd = createSocketAndConnect(SERVER_ADDR, SERVER_PORT);
    sendXmlRequest(sockfd, filePath);
    std::cout << "Response: " << receiveResponse(sockfd) << std::endl;
    close(sockfd);
}

int main(int argc, char** argv) {
    pthread_t threads[MAX_THREAD];
    for (int i = 0; i < MAX_THREAD; ++i) {
        pthread_create(&threads[i], NULL, handler, argv[1]);
        usleep(3000);
    }
    for (int i = 0; i < MAX_THREAD; ++i) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
