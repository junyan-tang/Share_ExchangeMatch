#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <chrono>
#include "global_var.hpp"
#include "myStruct.h"

using namespace std;
using namespace pqxx;

class Database
{
public:
    connection *C = root_connection;

    void drop_table();
    void create_table();
    void init_database();

    void insert_account(work & W,string account_id, double balance);
    void insert_stock(work & W, string stock_id, string account_id, double num);
    void insert_sell_order(work & W, string stock_id, string account_id, double num, double price, string timestamp, string trans_id);
    void insert_buy_order(work & W, string stock_id, string account_id, double num, double price, string timestamp, string trans_id);
    void insert_transaction(work & W, string trans_id, string timestamp, string account_id, string stock_id, double num, double price, string status);

    void update_account(work & W, string account_id, double balance);
    void update_stock(work & W, string stock_id, string account_id, double num);
    void update_sell_order(work & W,string stock_id, string account_id, double num, double price, string timestamp);
    void update_buy_order(work & W,string stock_id, string account_id, double num, double price, string timestamp);
    void update_transaction(work & W,string trans_id, string account_id, string stock_id, double num, double price, string status);
    void update_transaction(work & W,string trans_id, string timestamp, string status, double price);
    void update_transaction(work & W,string trans_id, string timestamp, string status);
    void update_transaction(work & W,string trans_id, double num);

    void delete_account(work & W,string account_id);
    void delete_stock(work & W,string stock_id);
    void delete_sell_order(work & W,string trans_id);
    void delete_buy_order(work & W,string trans_id);
    void delete_transaction(work & W,string trans_id);

    void show_table(string table_name);

    result inquire_account(string account_id);
    result inquire_stock(string stock_id, string account_id);
    result inquire_transaction(string trans_id);

    vector<Order> retrieve_sell_order();
    vector<Order> retrieve_buy_order();


    ResultC create_account(string account_id, double balance);
    ResultC create_stock(string stock_id, string account_id, double num);
    ResultT query_order(string trans_id);
    ResultT open_sell_order(string account_id, string sym, double amount, double limit, string trans_id);
    ResultT open_buy_order(string account_id, string sym, double amount, double price, string trans_id);
    ResultT cancel_order(string trans_id);

    string get_time();
};

#endif // DATABASE_HPP