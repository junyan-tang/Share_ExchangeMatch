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
    void sentStock(string account_id, string stock_id, double amount)
    {
        return mkt.sentStock(account_id, stock_id, amount);
    }
    void sentMoney(string account_id, double amount)
    {
        return mkt.sentMoney(account_id, amount);
    }
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