#ifndef SHARE_EXCHANGEMATCH_MYSTRUCT_H
#define SHARE_EXCHANGEMATCH_MYSTRUCT_H


#include <string>
#include <vector>
using namespace std;

struct Order {
    string stock_id;
    string account_id;
    int num;
    double price;
    string order_time; 
};

struct Transaction {
    string stock_id;
    string account_id;
    double num;
    double price;
    string order_time; 
    string status;
};


struct ResultC {
    string account_id;
    string sym;
    string status;
    string message;
};

struct ResultT{
    string account_id;
    string transaction_type;
    int trans_id;
    string sym;
    string status;
    string message;
    vector<Transaction> transaction;
};

// struct Transaction{
//     string transaction_id
//     vector<Order, status> orders;
//     string status;
// }

#endif