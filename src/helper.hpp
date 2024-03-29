#ifndef HELPER_HPP
#define HELPER_HPP

#include <pqxx/pqxx>
#include "sql_init/Database.h"
#include <string>

using namespace std;
using namespace pqxx;

class Transaction {
public:
    void openOrder(string sym, string amount, string limit);
    void cancelOrder(string transaction_id);
    void queryOrder(string transaction_id);

};

class Creation {
public:
    void createAccount(string account_id, string balance);
    void createStock(string sym, string account_id, int amount);
};

#endif