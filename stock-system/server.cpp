#include "server.hpp"
#include "parser.hpp"
  #include <functional>
Server::Server() : pool(10)
{
  memset(&host, 0, sizeof(host));
  host.ai_family = AF_UNSPEC;
  host.ai_socktype = SOCK_STREAM;
  host.ai_flags = AI_PASSIVE;
  status = getaddrinfo(NULL, PORT, &host, &host_list);

  if (status != 0)
  {
    cerr << "Error: address issue" << endl;
    exit(EXIT_FAILURE);
  }

  server_sockfd = socket(host_list->ai_family, host_list->ai_socktype,
                         host_list->ai_protocol);

  if (server_sockfd == -1)
  {
    cerr << "Error: socket creation failed" << endl;
    exit(EXIT_FAILURE);
  }

  int yes = 1;
  status =
      setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  if (status == -1)
  {
    cerr << "Error: socket operation fail" << endl;
    exit(EXIT_FAILURE);
  }

  status = bind(server_sockfd, host_list->ai_addr, host_list->ai_addrlen);
  if (status == -1)
  {
    cerr << "Error: Binding fail" << endl;
    exit(EXIT_FAILURE);
  }

  status = listen(server_sockfd, 10240);
  if (status == -1)
  {
    cerr << "Error: listen fail" << endl;
    exit(EXIT_FAILURE);
  }
};



string Server::recv_request(int socket_fd)
{
  const int bufferSize = 1024;
  char buffer[bufferSize];
  memset(buffer, 0, bufferSize);

  string lengthStr;
  while (true)
  {
    ssize_t bytesReceived = recv(socket_fd, buffer, 1, 0);
    if (bytesReceived <= 0)
    {
      cout << "receive length failed with value: " << bytesReceived << endl;
      return "receive length failed";
    }
    if (buffer[0] == '\n')
      break;
    lengthStr += buffer[0];
  }

  size_t expectedLength = stoull(lengthStr);
  cout << "Expected length: " << expectedLength << endl;

  string data;
  size_t totalBytesReceived = 0;
  while (totalBytesReceived < expectedLength)
  {
    size_t bytesToRead = std::min(static_cast<size_t>(bufferSize), expectedLength - totalBytesReceived);
    ssize_t bytesReceived = recv(socket_fd, buffer, bytesToRead, 0);

    if (bytesReceived <= 0)
    {
      cout << "Error: recv failed in loop" << endl;
      return "Error: recv failed in loop";
    }
    data.append(buffer, bytesReceived);
    totalBytesReceived += bytesReceived;
  }

  return data;
}

void Server::run()
{


  while (true)
  {
    cout << "watting for connection" << endl;
    sin_size = sizeof(their_addr);
    int new_fd = accept(server_sockfd, (struct sockaddr *)&their_addr, &sin_size);

    if (new_fd == -1)
    {
      cerr << "Error: accept fail" << endl;
      exit(EXIT_FAILURE);
    }
    auto processFunc = std::bind(&Server::process, this, new_fd);
    pool.commit(processFunc);
  }
}

void Server::process(int new_fd)
{


  int a = 0;
  while(a<10){
    string data = recv_request(new_fd);

    cout << "receive data in server: " << data << endl; 
    XMLParser parser;
    
    string re = parser.parseRequest(data);
    
    
    cout << "here is response: " << re << endl;
    send(new_fd, re.c_str(), re.length(), 0);
    a += 1;


  }





    
  close(new_fd); 

}

Server::~Server()
{
  freeaddrinfo(host_list);
  close(server_sockfd);
}