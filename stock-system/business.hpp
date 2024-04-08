#ifndef BUSINESS_HPP
#define BUSINESS_HPP

#include "database.hpp"
#include "myStruct.h"
#include <string>
#include "market.hpp"

using namespace std;
using namespace pqxx;

class Transact
{
private:
    Database db;
    market mkt = market(db);
    connection *C = root_connection;

public:
    ResultT openOrder(string account_id, string stock_id, string amount, string limit, string trans_id);
    ResultT cancelOrder(string trans_id);
    ResultT queryOrder(string trans_id);
    bool checkAccount(string account_id);
};

class Creation
{
private:
    Database db;
    connection *C = root_connection;

public:
    ResultC createAccount(string account_id, string balance);
    ResultC createStock(string sym, string account_id, double amount);
};

#endif