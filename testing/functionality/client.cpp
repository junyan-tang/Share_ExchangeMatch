#include <iostream>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

string resize_data(string data)
{

    size_t dataLength = data.length();
    data = to_string(dataLength) + "\n" + data;
    return data;
}

void send_data(int sockfd, const string &data)
{
    size_t totalBytesSent = 0;
    while (totalBytesSent < data.size())
    {
        ssize_t sent = send(sockfd, data.c_str() + totalBytesSent, data.size() - totalBytesSent, 0);
        if (sent == -1)
        {
            cerr << "Error: send failed" << endl;
        }
        totalBytesSent += sent;
    }
}

string recv_data(int sockfd)
{
    char buffer[8192];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == -1)
    {
        cerr << "Error: recv failed" << endl;
        return ""; // Error handling
    }
    else if (bytesReceived == 0)
    {
        cerr << "Connection closed" << endl;
        return "";
    }

    buffer[bytesReceived] = '\0';
    string re = string(buffer);
    cout << "this is response: " << re << endl;
    return re;
}

void test(int sockfd)
{
    string data;

    data = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<create>\n"
           "   <account id=\"123456\" balance=\"1000000\"/>\n"
           "   <account id=\"223456\" balance=\"1000000\"/>\n"
           "   <account id=\"323456\" balance=\"1000000\"/>\n"
           "   <account id=\"423456\" balance=\"1000000\"/>\n"
           "   <account id=\"523456\" balance=\"1000000\"/>\n"
           "   <account id=\"623456\" balance=\"1000000\"/>\n"
           "   <account id=\"723456\" balance=\"1000000\"/>\n"
           "   <symbol sym=\"SPY\">\n"
           "       <account id=\"123456\">10000</account>\n"
           "       <account id=\"223456\">10000</account>\n"
           "       <account id=\"323456\">10000</account>\n"
           "       <account id=\"423456\">10000</account>\n"
           "       <account id=\"523456\">10000</account>\n"
           "       <account id=\"623456\">10000</account>\n"
           "       <account id=\"723456\">10000</account>\n"
           "   </symbol>\n"
           "</create>\n";

    string data1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"123456\">\n"
                   "<order sym=\"SPY\" amount=\"300\" limit=\"125\"/>\n"
                   "<query id=\"78910\"/>\n"
                   "<cancel id=\"78910\"/>\n"
                   "</transactions>\n";

    string data2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"223456\">\n"
                   "<order sym=\"SPY\" amount=\"-100\" limit=\"130\"/>\n"
                   "<query id=\"1\"/>\n"
                   "</transactions>\n";

    string data3 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"323456\">\n"
                   "<order sym=\"SPY\" amount=\"200\" limit=\"127\"/>\n"
                   "<query id=\"1\"/>\n"
                   "</transactions>\n";

    string data4 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"423456\">\n"
                   "<order sym=\"SPY\" amount=\"-500\" limit=\"128\"/>\n"
                   "</transactions>\n";

    string data5 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"523456\">\n"
                   "<order sym=\"SPY\" amount=\"-200\" limit=\"140\"/>\n"
                   "</transactions>\n";

    string data6 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"623456\">\n"
                   "<order sym=\"SPY\" amount=\"400\" limit=\"125\"/>\n"
                   "</transactions>\n";

    string data7 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"323456\">\n"
                   "   <query id=\"1\"/>\n"
                   "   <query id=\"2\"/>\n"
                   "   <query id=\"3\"/>\n"
                   "   <query id=\"4\"/>\n"
                   "   <query id=\"5\"/>\n"
                   "   <query id=\"6\"/>\n"
                   "</transactions>\n";

    string data8 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"723456\">\n"
                   "   <order sym=\"SPY\" amount=\"-400\" limit=\"124\"/>\n"
                   "   <query id=\"1\"/>\n"
                   "   <query id=\"2\"/>\n"
                   "   <query id=\"3\"/>\n"
                   "   <query id=\"4\"/>\n"
                   "   <query id=\"5\"/>\n"
                   "   <query id=\"6\"/>\n"
                   "   <query id=\"7\"/>\n"
                   "</transactions>\n";

    string response;

    data = resize_data(data);

    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);
    data4 = resize_data(data4);
    data5 = resize_data(data5);
    data6 = resize_data(data6);
    data7 = resize_data(data7);
    data8 = resize_data(data8);

    send_data(sockfd, data);
    recv_data(sockfd);
    send_data(sockfd, data1);
    recv_data(sockfd);
    send_data(sockfd, data2);
    recv_data(sockfd);
    send_data(sockfd, data3);
    recv_data(sockfd);
    send_data(sockfd, data4);
    recv_data(sockfd);
    send_data(sockfd, data5);
    recv_data(sockfd);
    send_data(sockfd, data6);
    recv_data(sockfd);
    send_data(sockfd, data7);
    recv_data(sockfd);
    send_data(sockfd, data8);
    recv_data(sockfd);
}

void test1(int sockfd)
{
    string data1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"123456\">\n"
                   "<order sym=\"SPY\" amount=\"200\" limit=\"127\"/>\n"
                   "<query id=\"78910\"/>\n"
                   "<cancel id=\"78910\"/>\n"
                   "</transactions>\n";

    string data2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"223456\">\n"
                   "<order sym=\"SPY\" amount=\"300\" limit=\"125\"/>\n"
                   "<query id=\"1\"/>\n"
                   "</transactions>\n";

    string data3 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"323456\">\n"
                   "<order sym=\"SPY\" amount=\"400\" limit=\"125\"/>\n"
                   "<cancel id=\"1\"/>\n"
                   "</transactions>\n";

    string data4 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"423456\">\n"
                   "<order sym=\"SPY\" amount=\"-200\" limit=\"140\"/>\n"
                   "</transactions>\n";
    string data5 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"523456\">\n"
                   "<order sym=\"SPY\" amount=\"-100\" limit=\"130\"/>\n"
                   "</transactions>\n";
    string data6 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"623456\">\n"
                   "<order sym=\"SPY\" amount=\"-500\" limit=\"128\"/>\n"
                   "</transactions>\n";
    string data7 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"723456\">\n"
                   "   <order sym=\"SPY\" amount=\"-400\" limit=\"124\"/>\n"
                   "</transactions>\n";

    string data8 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<transactions id=\"723456\">\n"
                   "   <query id=\"1\"/>\n"
                   "   <query id=\"2\"/>\n"
                   "   <query id=\"3\"/>\n"
                   "   <query id=\"4\"/>\n"
                   "   <query id=\"5\"/>\n"
                   "   <query id=\"6\"/>\n"
                   "   <query id=\"7\"/>\n"
                   "</transactions>\n";

    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);
    data4 = resize_data(data4);
    data5 = resize_data(data5);
    data6 = resize_data(data6);
    data7 = resize_data(data7);
    data8 = resize_data(data8);

    send_data(sockfd, data1);
    recv_data(sockfd);
    send_data(sockfd, data2);
    recv_data(sockfd);
    send_data(sockfd, data3);
    recv_data(sockfd);
    send_data(sockfd, data4);
    recv_data(sockfd);
    send_data(sockfd, data5);
    recv_data(sockfd);
    send_data(sockfd, data6);
    recv_data(sockfd);
    send_data(sockfd, data7);
    recv_data(sockfd);
    send_data(sockfd, data8);
    recv_data(sockfd);
}

void test2(int sockfd)
{
    string data = "<create><account id=\"0\" balance=\"50000\"/><account id=\"2\" balance=\"-100000\"/><symbol sym=\"TESLA\"><account id=\"1\">200</account></symbol></create>";
    data = resize_data(data);
    send_data(sockfd, data);
    recv_data(sockfd);
}

void test3(int sockfd)
{
    string data = "<create><account id=\"2\" balance=\"50000\"/><symbol sym=\"TB\"><account id=\"2\">500</account></symbol></create>";
    string data1 = "<transactions id=\"2\"><order sym=\"TESLA\" amount=\"100\" limit=\"250\"/><order sym=\"TB\" amount=\"-100\" limit=\"120\"/><query id=\"1\"/><query id=\"2\"/></transactions>";
    string data2 = "<transactions id=\"2\"><query id=\"1\"/></transactions>";
    string data3 = "<transactions id=\"2\"><cancel id=\"1\"/><cancel id=\"2\"/><query id=\"1\"/><query id=\"2\"/></transactions>";

    data = resize_data(data);
    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);

    send_data(sockfd, data);
    recv_data(sockfd);
    send_data(sockfd, data1);
    recv_data(sockfd);
    send_data(sockfd, data2);
    recv_data(sockfd);
    send_data(sockfd, data3);
    recv_data(sockfd);
}

void test4(int sockfd)
{
    string data = "<create><account id=\"0\" balance=\"50000\"/><account id=\"2\" balance=\"-100000\"/><symbol sym=\"TESLA\"><account id=\"1\">200</account></symbol></create>";
    string data1 = "<create><account id=\"1\" balance=\"50000\"/><symbol sym=\"TESLA\"><account id=\"1\">500</account></symbol></create>";
    string data2 = "<transactions id=\"1\"><query id=\"1\"/><cancel id=\"1\"/></transactions>";
    string data3 = "<transactions id=\"1000\"><order sym=\"TESLA\" amount=\"100\" limit=\"250\"/><query id=\"1\"/><cancel id=\"1\"/></transactions>";

    data = resize_data(data);
    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);

    send_data(sockfd, data);
    recv_data(sockfd);
    send_data(sockfd, data1);
    recv_data(sockfd);
    send_data(sockfd, data2);
    recv_data(sockfd);
    send_data(sockfd, data3);
    recv_data(sockfd);
}

void test5(int sockfd)
{
    string data = "<create><account id=\"1\" balance=\"100000\"/><account id=\"2\" balance=\"100000\"/><account id=\"3\" balance=\"100000\"/><account id=\"4\" balance=\"100000\"/><account id=\"5\" balance=\"100000\"/><account id=\"6\" balance=\"100000\"/><account id=\"7\" balance=\"100000\"/><symbol sym=\"X\"><account id=\"1\">500</account><account id=\"2\">500</account><account id=\"3\">500</account><account id=\"4\">500</account><account id=\"5\">500</account><account id=\"6\">500</account><account id=\"7\">500</account></symbol></create>";
    string data1 = "<transactions id=\"1\"><order sym=\"X\" amount=\"300\" limit=\"125\"/></transactions>";
    string data2 = "<transactions id=\"2\"><order sym=\"X\" amount=\"-100\" limit=\"130\"/></transactions>";
    string data3 = "<transactions id=\"3\"><order sym=\"X\" amount=\"200\" limit=\"127\"/></transactions>";
    string data4 = "<transactions id=\"4\"><order sym=\"X\" amount=\"-500\" limit=\"128\"/></transactions>";
    string data5 = "<transactions id=\"5\"><order sym=\"X\" amount=\"-200\" limit=\"140\"/></transactions>";
    string data6 = "<transactions id=\"6\"><order sym=\"X\" amount=\"400\" limit=\"125\"/></transactions>";
    string data7 = "<transactions id=\"7\"><order sym=\"X\" amount=\"-400\" limit=\"124\"/></transactions>";

    data = resize_data(data);
    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);
    data4 = resize_data(data4);
    data5 = resize_data(data5);
    data6 = resize_data(data6);
    data7 = resize_data(data7);

    send_data(sockfd, data);
    recv_data(sockfd);
    send_data(sockfd, data1);
    recv_data(sockfd);
    send_data(sockfd, data2);
    recv_data(sockfd);
    send_data(sockfd, data3);
    recv_data(sockfd);
    send_data(sockfd, data4);
    recv_data(sockfd);
    send_data(sockfd, data5);
    recv_data(sockfd);
    send_data(sockfd, data6);
    recv_data(sockfd);
    send_data(sockfd, data7);
    recv_data(sockfd);
}

void test6(int sockfd)
{
    string data = "<create><account id=\"2\" balance=\"50000\"/><symbol sym=\"TB\"><account id=\"2\">500</account></symbol></create>";
    string data1 = "<transactions id=\"2\"><order sym=\"TESLA\" amount=\"100\" limit=\"250\"/><order sym=\"TB\" amount=\"-100\" limit=\"120\"/><query id=\"1\"/><query id=\"2\"/></transactions>";
    string data2 = "<transactions id=\"2\"><cancel id=\"1\"/><cancel id=\"2\"/><query id=\"1\"/></transactions>";
    string data3 = "<transactions id=\"2\"><cancel id=\"1\"/><cancel id=\"2\"/><query id=\"1\"/><query id=\"2\"/></transactions>";

    data = resize_data(data);
    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);

    send_data(sockfd, data);
    recv_data(sockfd);
    send_data(sockfd, data1);
    recv_data(sockfd);
    send_data(sockfd, data2);
    recv_data(sockfd);
    send_data(sockfd, data3);
    recv_data(sockfd);
}

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("127.0.0.1", "12345", &hints, &res);
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        cerr << "Error: could not connect" << endl;
        return 1;
    }

<<<<<<< HEAD


    test5(sockfd);
=======
    test(sockfd);
>>>>>>> 696abeebb98ae7e4689be7734120674fb166965e

    close(sockfd);
    freeaddrinfo(res);

    return 0;
}
