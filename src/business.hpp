#ifndef BUSINESS_HPP
#define BUSINESS_HPP

#include "database.cpp"
#include "myStruct.h"
#include <string>

using namespace std;
using namespace pqxx;

class Transaction {
private:
    Database db;
public:
    void openOrder(string account_id, string stock_id, string amount, string limit);
    void cancelOrder(string transaction_id);
    void queryOrder(string transaction_id);
};

class Creation {
private:
    Database db;
public:
    ResultC createAccount(string account_id, string balance);
    ResultC createStock(string sym, string account_id, double amount);
};

#endif