#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include "global_var.hpp"

using namespace std;
using namespace pqxx;

class Database{


public:
    connection *C = root_connection;
    void drop_table();
    void create_table();
    void init_database();



    void insert_account(string account_id, double balance);
    void insert_stock(string stock_id, string account_id, double num);
    void insert_sell_order(string stock_id, string account_id, double num, double price, string timestamp);
    void insert_buy_order(string stock_id, string account_id, double num, double price, string timestamp);
    void insert_transaction(int transaction_id, string timestamp, string account_id, string stock_id, double num, double price, string status);



    void update_account(string account_id, double balance);
    void update_stock(string stock_id, string account_id, double num);
    void update_sell_order(string stock_id, string account_id, double num, double price,string timestamp);
    void update_buy_order(string stock_id, string account_id, double num, double price,string timestamp);
    void update_transaction(int transaction_id, string account_id, string stock_id, double num, double price, string status);
    void update_transaction(int transaction_id, string timestamp, string status);

    void delete_account(string account_id);
    void delete_stock(string stock_id);
    void delete_sell_order(string stock_id, string account_id);
    void delete_buy_order(string stock_id, string account_id);
    void delete_transaction(int transaction_id);

    void show_table(string table_name);

    result inquire_account(string account_id);
    result inquire_stock(string stock_id, string account_id);
    result inquire_transaction(int transaction_id);

};

#endif // DATABASE_HPP