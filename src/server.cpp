#include "server.hpp"
#include "parser.hpp"




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
  db.show_table("ACCOUNT");
  db.show_table("SHARE");
  while (true) {
    cout << "watting for connection" << endl;
    process();
  }
}


string Server::recv_request(int new_fd) {

    char buf[1024];
    memset(buf, 0, 1024); 

    string data;
    size_t totalBytesReceived = 0;
    bool headerParsed = false;
    size_t expectedLength = 0; 

    while (true) {
        int bytesReceived = recv(new_fd, buf, 1024, 0);

        if (bytesReceived == -1) {
            cerr << "Error: recv failed" << endl;
            exit(EXIT_FAILURE);
        }

        data.append(buf, bytesReceived);
        totalBytesReceived += bytesReceived;

        if (!headerParsed) {
            size_t endOfLength = data.find("\n");
            if (endOfLength != string::npos) {
                expectedLength = stoull(data.substr(0, endOfLength));
                headerParsed = true;
                data.erase(0, endOfLength + 1); 
                totalBytesReceived -= (endOfLength + 1); 
            }
        }

        if (headerParsed && (totalBytesReceived >= expectedLength || data.find("</create>") != string::npos)) {
            break; 
        }

        memset(buf, 0, bytesReceived);
    }

    return data;
}


void Server::process() {
  sin_size = sizeof(their_addr);
  int new_fd = accept(server_sockfd, (struct sockaddr *)&their_addr, &sin_size);


  if (new_fd == -1) {
    cerr << "Error: accept fail" << endl;
    exit(EXIT_FAILURE);
  }

  while(true){
    string data = recv_request(new_fd);

    cout << "receive data in server: " << data << endl;
    XMLParser parser;

    string re = parser.parseRequest(data);

    cout << "here is response: " << re << endl;
  }


  close(new_fd);
  
}

void Server::handle_create(ResultC res) {
  cout << "Handling create" << endl;
  cout << "Account ID: " << res.account_id << endl;
  cout << "Symbol: " << res.sym << endl;
  cout << "Ammount: " << res.status << endl;
  cout << "Message: " << res.message << endl;
}

void Server::handle_transaction(ResultT res) {
  cout << "Handling transaction" << endl;
  cout << "Account ID: " << res.account_id << endl;
  cout << "Transaction Type: " << res.transaction_type << endl;
  cout << "Transaction id: " << res.trans_id << endl;
  cout << "Symbol: " << res.sym << endl;
  cout << "Status: " << res.status << endl;
  cout << "Message: " << res.message << endl;
}

Server::~Server() {
  freeaddrinfo(host_list);
  close(server_sockfd);
}
