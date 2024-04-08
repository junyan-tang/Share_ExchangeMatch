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

// Change MAX_THREAD to increase or decrease the number of queries sent
#define MAX_THREAD 1
#define BUFF_SIZE 10240

// Struct to pass multiple arguments to the handler function
struct ThreadArgs {
    std::string server_addr;
    std::string server_port;
    std::string file_path;
};

// Handler function to set up the client and start sending message to server
void *handler(void *arguments) {
    char buffer[BUFF_SIZE];
    int server_sfd;
    struct addrinfo hints, *server_info;

    // Cast argument back to correct type
    ThreadArgs* args = static_cast<ThreadArgs*>(arguments);
    std::string server_addr = args->server_addr;
    std::string server_port = args->server_port;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int status = getaddrinfo(server_addr.c_str(), server_port.c_str(), &hints, &server_info);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << '\n';
        return nullptr;
    }

    // Create socket
    server_sfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (server_sfd < 0) {
        perror("socket");
        return nullptr;
    }

    // Connect to the server
    status = connect(server_sfd, server_info->ai_addr, server_info->ai_addrlen);
    if (status < 0) {
        perror("connect");
        close(server_sfd);
        return nullptr;
    }

    // Read XML file content to send
    std::ifstream fs(args->file_path);
    std::stringstream ss;
    std::string req;

    if (fs.is_open()) {
        ss << fs.rdbuf(); // Read the entire file
        req = ss.str(); // Get the string from stringstream
    } else {
        std::cerr << "Could not open file: " << args->file_path << '\n';
        close(server_sfd);
        return nullptr;
    }

    // Prefix the request with its length
    std::string prefix = std::to_string(req.length()) + "\n";
    req = prefix + req;

    // Send the request
    send(server_sfd, req.c_str(), req.size(), 0);

    // Receive the response
    recv(server_sfd, buffer, BUFF_SIZE, 0);

    // Close the socket
    close(server_sfd);
    
    // Free the server_info structure
    freeaddrinfo(server_info);
    
    return nullptr;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_address> <server_port> <file_path>\n";
        return 1;
    }

    // Extract server info and file path from arguments
    std::string server_addr = argv[1];
    std::string server_port = argv[2];
    std::string file_path = argv[3];

    // Thread setup
    pthread_t threads[MAX_THREAD];
    ThreadArgs args = {server_addr, server_port, file_path};

    // Create threads to send messages
    for (int i = 0; i < MAX_THREAD; ++i) {
        if (pthread_create(&threads[i], NULL, handler, &args)) {
            std::cerr << "Failed to create thread " << i << '\n';
        }
        usleep(1000); // Sleep for thread setup
    }

    // Join threads
    for (int i = 0; i < MAX_THREAD; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
