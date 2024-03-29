#include "server.hpp"
#include <cstring>
#include <iostream>

using namespace std;


Server::Server() {
  memset(&host, 0, sizeof(host));
  host.ai_family = AF_UNSPEC;
  host.ai_socktype = SOCK_STREAM;
  host.ai_flags = AI_PASSIVE;
  status = getaddrinfo(NULL, PORT, &host, &host_list);

  if (status != 0) {
    cerr << "Error: address issue" << endl;
    exit(EXIT_FAILURE);
  }

  server_sockfd = socket(host_list->ai_family, host_list->ai_socktype,
                         host_list->ai_protocol);

  if (server_sockfd == -1) {
    cerr << "Error: socket creation failed" << endl;
    exit(EXIT_FAILURE);
  }

  int yes = 1;
  status =
      setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  if (status == -1) {
    cerr << "Error: socket operation fail" << endl;
    exit(EXIT_FAILURE);
  }

  status = bind(server_sockfd, host_list->ai_addr, host_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: Binding fail" << endl;
    exit(EXIT_FAILURE);
  }

  status = listen(server_sockfd, 10240);
  if (status == -1) {
    cerr << "Error: listen fail" << endl;
    exit(EXIT_FAILURE);
  }
};

void Server::run() {
  while (true) {
    process();
  }
}

void Server::process() {
  sin_size = sizeof(their_addr);
  int new_fd = accept(server_sockfd, (struct sockaddr *)&their_addr, &sin_size);
  if (new_fd == -1) {
    cerr << "Error: accept fail" << endl;
    exit(EXIT_FAILURE);
  }

  char buf[1024];
  int numbytes = recv(new_fd, buf, 1024, 0);
  if (numbytes == -1) {
    cerr << "Error: recv fail" << endl;
    exit(EXIT_FAILURE);
  }

  buf[numbytes] = '\0';
  cout << "Received: " << buf << endl;

  close(new_fd);
}

string Server::recv_request(int new_fd) {
    string res;
    char buffer[1024];
    size_t expect_bytes = recv(new_fd, &(buffer[0]), 1024, 0);

    size_t cur_bytes;



    
    return res;


}
