#include <iostream>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;


string resize_data(string data){
    // size_t dataLength = data.size();
    size_t dataLength = sizeof(data);
    data = to_string(dataLength) + "\n" + data;
    return data;
}

void send_data(int sockfd, const string& data) {
    size_t totalBytesSent = 0;
    while (totalBytesSent < data.size()) {
        ssize_t sent = send(sockfd, data.c_str() + totalBytesSent, data.size() - totalBytesSent, 0);
        if (sent == -1) {
            cerr << "Error: send failed" << endl;
            exit(EXIT_FAILURE);
        }
        totalBytesSent += sent;
    }
}

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    getaddrinfo("127.0.0.1", "12345", &hints, &res);
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        cerr << "Error: could not connect" << endl;
        return 1;
    }

    string data;
    //data = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<create>\n<account id=\"123456\" balance=\"1000\"/>\n<symbol sym=\"SPY\">\n<account id=\"123456\">100000</account>\n</symbol>\n</create>\n";
    
    data =   "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<create>\n"
            "<account id=\"123456\" balance=\"1000\"/>\n"
            "<symbol sym=\"SPY\">\n"
            "<account id=\"123456\">-10000</account>\n"
            "<account id=\"223456\">10000</account>\n"
            "</symbol>\n"
            "</create>\n";
    data = resize_data(data);
    send_data(sockfd, data);
    cout << data << "\n======" << endl;
    
    string data1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<transactions id=\"123456\">\n"
            "<order sym=\"SPY\" amount=\"100\" limit=\"300\"/>\n"
            "<query id=\"78910\"/>\n"
            "<cancel id=\"78910\"/>\n"
            "</transactions>\n";
    data1 = resize_data(data1);
    send_data(sockfd, data1);
    cout << data1 << "\n======" << endl;

    string data2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"123456\">\n"
        "<order sym=\"SPY\" amount=\"100\" limit=\"300\"/>\n"
        "<query id=\"1\"/>\n"
        "</transactions>\n";

    data2 = resize_data(data2);

    send_data(sockfd, data2);
    cout << data2 << "\n======" << endl;

    close(sockfd);
    freeaddrinfo(res);

    return 0;
}


