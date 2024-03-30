#ifndef SHARE_EXCHANGEMATCH_MYSTRUCT_H
#define SHARE_EXCHANGEMATCH_MYSTRUCT_H


#include <string>
using namespace std;

struct ResultC {
    string account_id;
    string sym;
    string status;
    string message;
};

struct ResultT{

};

struct Order {
    string stock_id;
    string account_id;
    int num;
    double price;
    string order_time; 
};


#endif