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
#include <vector>
#include <future> // Include the necessary header file
#include "threadpool.hpp"
std::string resize_data(std::string data){

    size_t dataLength = data.length();
    data = std::to_string(dataLength) + "\n" + data;
    return data;
}


// Change MAX_THREAD to increase or decrease the number of queries sent
#define MAX_THREAD 1
#define BUFF_SIZE 10240

// Struct to pass multiple arguments to the handler function
struct ThreadArgs {
    std::string server_addr;
    std::string server_port;
    std::string file_path;
    int thread_id;
};
void* threadHandler(void* arguments) {
    ThreadArgs* args = static_cast<ThreadArgs*>(arguments);
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char buffer[BUFF_SIZE];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(args->server_addr.c_str(), args->server_port.c_str(), &hints, &servinfo)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << "\n";
        return nullptr;
    }

    // Loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break; // If we get here, we must have connected successfully
    }

    if (p == NULL) {
        std::cerr << "client: failed to connect\n";
        return nullptr;
    }

    freeaddrinfo(servinfo); // All done with this structure

    // Read the file content
    std::ifstream fs(args->file_path);
    std::stringstream ss;
    if (fs.is_open()) {
        ss << fs.rdbuf();
    } else {
        std::cerr << "Unable to open file: " << args->file_path << "\n";
        return nullptr;
    }



    std::string msg = ss.str();

    msg = resize_data(msg);
    std::cout << "send message" << endl;
    int len = send(sockfd, msg.c_str(), msg.size(), 0);


    if (len == -1) {
        perror("send");
        close(sockfd);
        return nullptr;
    }

    // Receive the response
    len = recv(sockfd, buffer, sizeof(buffer), 0);
    if (len == -1) {
        perror("recv");
        close(sockfd);
        return nullptr;
    }

    // Print out the buffer content
    std::cout << "Thread " << args->thread_id << " received: " << std::string(buffer, len) << "\n";

    close(sockfd); // Close the socket

    return nullptr;
}

void serializedExecution(const std::string& server_addr, const std::string& server_port, const std::string& file_path, int num_threads) {
    std::vector<double> elapsedTimes;
    std::ofstream result_file("serialized_results.txt", std::ios::out); 


    for (int i = 0; i < num_threads; ++i) {
        result_file << "serialize" << i + 1;
        if (i < num_threads - 1) result_file << ",";
    }
    result_file << "\n";

    for (int i = 0; i < num_threads; ++i) {
        ThreadArgs* args = new ThreadArgs{server_addr, server_port, file_path, i};

        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "begin thread " << i << std::endl;
        threadHandler(args); 
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        elapsedTimes.push_back(elapsed.count());
    }


    for (auto& time : elapsedTimes) {
        result_file << time;
        if (&time != &elapsedTimes.back()) result_file << ",";
    }
    result_file << "\n";
    result_file.close();
}
void concurrentExecution(const std::string& server_addr, const std::string& server_port, const std::string& file_path, int num_threads) {
    std::vector<std::future<double>> futures;
    std::vector<double> elapsedTimes;
    Threadpool pool(num_threads); 

    std::ofstream result_file("concurrent_results.txt", std::ios::out); 


    for (int i = 0; i < num_threads; ++i) {
        result_file << "concurrent" << i + 1;
        if (i < num_threads - 1) result_file << ",";
    }
    result_file << "\n";

    for (int i = 0; i < num_threads; ++i) {
        futures.push_back(pool.enqueue([server_addr, server_port, file_path, i]() -> double {
            auto start = std::chrono::high_resolution_clock::now();
            ThreadArgs args = {server_addr, server_port, file_path, i};
            threadHandler(&args); 
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            return elapsed.count();
        }));
    }

    for (auto& future : futures) {
        double time = future.get();
        elapsedTimes.push_back(time);
    }


    for (auto& time : elapsedTimes) {
        result_file << time;
        if (&time != &elapsedTimes.back()) result_file << ",";
    }
    result_file << "\n";
    result_file.close();
}

int main(int argc, char **argv) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <server_address> <server_port> <file_path> <num_threads>\n";
        return 1;
    }

    std::string server_addr = argv[1];
    std::string server_port = argv[2];
    std::string file_path = argv[3];
    int num_threads = std::stoi(argv[4]);


    auto serialized_start = std::chrono::high_resolution_clock::now();
    std::cout << "begin serialized execution\n" << std::endl;
    serializedExecution(server_addr, server_port, file_path, num_threads);
    auto serialized_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> serialized_elapsed = serialized_end - serialized_start;
    std::cout << "Total serialized time: " << serialized_elapsed.count() << " seconds\n";


    // auto concurrent_start = std::chrono::high_resolution_clock::now();
    // concurrentExecution(server_addr, server_port, file_path, num_threads);
    // auto concurrent_end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> concurrent_elapsed = concurrent_end - concurrent_start;
    // std::cout << "Total concurrent time: " << concurrent_elapsed.count() << " seconds\n";

    return 0;
}

