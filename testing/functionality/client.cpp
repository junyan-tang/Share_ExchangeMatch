#include <iostream>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;


string resize_data(string data){

    size_t dataLength = data.length();
    data = to_string(dataLength) + "\n" + data;
    return data;
}

void send_data(int sockfd, const string& data) {
    size_t totalBytesSent = 0;
    while (totalBytesSent < data.size()) {
        ssize_t sent = send(sockfd, data.c_str() + totalBytesSent, data.size() - totalBytesSent, 0);
        if (sent == -1) {
            cerr << "Error: send failed" << endl;
        }
        totalBytesSent += sent;
    }
}


void test(int sockfd){
        string data;

    data =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
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

    data = resize_data(data);
    send_data(sockfd, data);
    cout << data << "\n======" << endl;
    
    string data1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<transactions id=\"123456\">\n"
            "<order sym=\"SPY\" amount=\"300\" limit=\"125\"/>\n"
            "<query id=\"78910\"/>\n"
            "<cancel id=\"78910\"/>\n"
            "</transactions>\n";
    data1 = resize_data(data1);
    send_data(sockfd, data1);
    cout << data1 << "\n======" << endl;

    string data2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"223456\">\n"
        "<order sym=\"SPY\" amount=\"-100\" limit=\"130\"/>\n"
        "<query id=\"1\"/>\n"
        "</transactions>\n";

    data2 = resize_data(data2);

    send_data(sockfd, data2);
    cout << data2 << "\n======" << endl;

    string data3 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"323456\">\n"
        "<order sym=\"SPY\" amount=\"200\" limit=\"127\"/>\n"
        "<query id=\"1\"/>\n"
        "</transactions>\n";

    data3 = resize_data(data3);

    send_data(sockfd, data3);
    cout << data3 << "\n======" << endl;

    string data4 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"423456\">\n"
        "<order sym=\"SPY\" amount=\"-500\" limit=\"128\"/>\n"
        "</transactions>\n";
    data4 = resize_data(data4);

    send_data(sockfd, data4);
    cout << data4 << "\n======" << endl;

    string data5 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"523456\">\n"
        "<order sym=\"SPY\" amount=\"-200\" limit=\"140\"/>\n"
        "</transactions>\n";
    data5 = resize_data(data5);

    send_data(sockfd, data5);
    cout << data5 << "\n======" << endl;

    string data6 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"623456\">\n"
        "<order sym=\"SPY\" amount=\"400\" limit=\"125\"/>\n"
        "</transactions>\n";
    data6 = resize_data(data6);

    send_data(sockfd, data6);
    cout << data6 << "\n======" << endl;

    string data7 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<transactions id=\"323456\">\n"
                "   <query id=\"1\"/>\n"
                "   <query id=\"2\"/>\n"
                "   <query id=\"3\"/>\n"
                "   <query id=\"4\"/>\n"
                "   <query id=\"5\"/>\n"
                "   <query id=\"6\"/>\n"
                "</transactions>\n";

    data7 = resize_data(data7);

    send_data(sockfd, data7);
    cout << data7 << "\n======" << endl;

    string data8 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                    "<transactions id=\"723456\">\n"
                    "   <order sym=\"SPY\" amount=\"-400\" limit=\"124\"/>\n"
                    "   <query id=\"1\"/>\n"
                    "   <query id=\"2\"/>\n"
                    "   <query id=\"3\"/>\n"
                    "   <query id=\"4\"/>\n"
                    "   <query id=\"5\"/>\n"
                    "   <query id=\"6\"/>\n"
                    "</transactions>\n";

    data8 = resize_data(data8);

    send_data(sockfd, data8);
    cout << data8 << "\n======" << endl;


}
void test1(int sockfd){


 

    
    string data1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<transactions id=\"123456\">\n"
            "<order sym=\"SPY\" amount=\"200\" limit=\"127\"/>\n"
            "<query id=\"78910\"/>\n"
            "<cancel id=\"78910\"/>\n"
            "</transactions>\n";
    data1 = resize_data(data1);

    cout << data1 << "\n======" << endl;

    send_data(sockfd, data1);



    string data2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"223456\">\n"
        "<order sym=\"SPY\" amount=\"300\" limit=\"125\"/>\n"
        "<query id=\"1\"/>\n"
        "</transactions>\n";

    data2 = resize_data(data2);


    cout << data2 << "\n======" << endl;
    send_data(sockfd, data2);


    string data3 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"323456\">\n"
        "<order sym=\"SPY\" amount=\"400\" limit=\"125\"/>\n"
        "<cancel id=\"1\"/>\n"
        "</transactions>\n";

    try
    {
        data3 = resize_data(data3);
        cout << data3 << "\n======" << endl;
        send_data(sockfd, data3);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    





    string data4 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"423456\">\n"
        "<order sym=\"SPY\" amount=\"-200\" limit=\"140\"/>\n"
        "</transactions>\n";
    data4 = resize_data(data4);
    cout << data4 << "\n======" << endl;
    send_data(sockfd, data4);



    string data5 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"523456\">\n"
        "<order sym=\"SPY\" amount=\"-100\" limit=\"130\"/>\n"
        "</transactions>\n";
    data5 = resize_data(data5);
    cout << data5 << "\n======" << endl;
    send_data(sockfd, data5);

        sleep(1);

    string data6 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<transactions id=\"623456\">\n"
        "<order sym=\"SPY\" amount=\"-500\" limit=\"128\"/>\n"
        "</transactions>\n";
    data6 = resize_data(data6);
    cout << data6 << "\n======" << endl;
    send_data(sockfd, data6);

        sleep(1);

    string data7 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<transactions id=\"723456\">\n"
                "   <order sym=\"SPY\" amount=\"-400\" limit=\"124\"/>\n"
                "</transactions>\n";

    data7 = resize_data(data7);
    cout << data7 << "\n======" << endl;
    send_data(sockfd, data7);



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

    data8 = resize_data(data8);
    cout << data8 << "\n======" << endl;
    send_data(sockfd, data8);



}


void test2(int sockfd){
    string data = "<create><account id=\"0\" balance=\"50000\"/><account id=\"2\" balance=\"-100000\"/><symbol sym=\"TESLA\"><account id=\"1\">200</account></symbol></create>";
    data = resize_data(data);
    send_data(sockfd, data);


}

//number of error ??? 
void test3(int sockfd){
    string data =   "<create><account id=\"2\" balance=\"50000\"/><symbol sym=\"TB\"><account id=\"2\">500</account></symbol></create>";
    string data1 = "<transactions id=\"2\"><order sym=\"TESLA\" amount=\"100\" limit=\"250\"/><order sym=\"TB\" amount=\"-100\" limit=\"120\"/><query id=\"1\"/><query id=\"2\"/></transactions>";
    string data2 = "<transactions id=\"2\"><query id=\"1\"/></transactions>";
    string data3 = "<transactions id=\"2\"><cancel id=\"1\"/><cancel id=\"2\"/><query id=\"1\"/><query id=\"2\"/></transactions>";

    data = resize_data(data);
    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);

    send_data(sockfd, data);
    send_data(sockfd, data1);
    send_data(sockfd, data2);
    send_data(sockfd, data3);
}

void test4(int sockfd){
    string data = "<create><account id=\"0\" balance=\"50000\"/><account id=\"2\" balance=\"-100000\"/><symbol sym=\"TESLA\"><account id=\"1\">200</account></symbol></create>";
    string data1 = "<create><account id=\"1\" balance=\"50000\"/><symbol sym=\"TESLA\"><account id=\"1\">500</account></symbol></create>";
    string data2 = "<transactions id=\"1\"><query id=\"1\"/><cancel id=\"1\"/></transactions>";
    string data3 = "<transactions id=\"1000\"><order sym=\"TESLA\" amount=\"100\" limit=\"250\"/><query id=\"1\"/><cancel id=\"1\"/></transactions>";

    data = resize_data(data);
    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);


    send_data(sockfd, data);
    send_data(sockfd, data1);
    send_data(sockfd, data2);
    send_data(sockfd, data3);
}

void test5(int sockfd) {
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
    send_data(sockfd, data1);
    send_data(sockfd, data2);
    send_data(sockfd, data3);
    send_data(sockfd, data4);
    send_data(sockfd, data5);
    send_data(sockfd, data6);
    send_data(sockfd, data7);
}

void test6(int sockfd) {
    string data = "<create><account id=\"2\" balance=\"50000\"/><symbol sym=\"TB\"><account id=\"2\">500</account></symbol></create>";
    string data1 = "<transactions id=\"2\"><order sym=\"TESLA\" amount=\"100\" limit=\"250\"/><order sym=\"TB\" amount=\"-100\" limit=\"120\"/><query id=\"1\"/><query id=\"2\"/></transactions>";
    string data2 = "<transactions id=\"2\"><cancel id=\"1\"/><cancel id=\"2\"/><query id=\"1\"/></transactions>";
    string data3 = "<transactions id=\"2\"><cancel id=\"1\"/><cancel id=\"2\"/><query id=\"1\"/><query id=\"2\"/></transactions>";


    data = resize_data(data);
    data1 = resize_data(data1);
    data2 = resize_data(data2);
    data3 = resize_data(data3);

    send_data(sockfd, data);
    send_data(sockfd, data1);
    send_data(sockfd, data2);
    send_data(sockfd, data3);



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



    test(sockfd);

    close(sockfd);
    freeaddrinfo(res);

    return 0;
}


