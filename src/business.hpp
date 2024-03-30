#ifndef BUSINESS_HPP
#define BUSINESS_HPP

#include "database.hpp"
#include "myStruct.h"
#include <string>

using namespace std;
using namespace pqxx;

class Transact {
private:
    Database db;
public:
    ResultT openOrder(string account_id, string stock_id, string amount, string limit, int trans_id);
    ResultT cancelOrder(int trans_id);
    ResultT queryOrder(int trans_id);
    bool checkAccount(string account_id);
};

class Creation {
public:
    Database db;
    
    ResultC createAccount(string account_id, string balance);
    ResultC createStock(string sym, string account_id, double amount);
};

#endif