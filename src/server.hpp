#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>


#define PORT "12345"
class Server{
private:




    int server_sockfd;
    int status;
    struct addrinfo host;
    struct addrinfo *host_list;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

public:
    Server();
    void run();
    void process();
    string recv_request(int new_fd);


    ~Server();



};