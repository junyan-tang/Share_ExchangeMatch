#include <string>


#include <iostream>
#include <pqxx/pqxx>
#include <fstream>

using namespace std;

using namespace pqxx;
class Database{
    public:
    void drop_table(connection *C);
    void create_table(connection *C);
    void init_database(connection *C);

    void insert_account(connection *C, string account_id, string balance);
    void insert_stock(connection *C, string stock_id, string account_id, double num);
    void insert_sell_order(connection *C, string stock_id, string account_id, double num, double price);
    void insert_buy_order(connection *C, string stock_id, string account_id, double num, double price);
    void insert_transaction(connection *C, string transaction_id, string account_id, string stock_id, double num, double price, bool isSell);



    void update_account(connection *C, string account_id, string balance);
    void update_stock(connection *C, string stock_id, string account_id, double num);
    void update_sell_order(connection *C, string stock_id, string account_id, double num, double price);
    void update_buy_order(connection *C, string stock_id, string account_id, double num, double price);
    void update_transaction(connection *C, string transaction_id, string account_id, string stock_id, double num, double price, bool isSell);


    void delete_account(connection *C, string account_id);
    void delete_stock(connection *C, string stock_id);
    void delete_sell_order(connection *C, string stock_id, string account_id);
    void delete_buy_order(connection *C, string stock_id, string account_id);
    void delete_transaction(connection *C, string transaction_id);

    
    void show_table(connection *C, string table_name);
};

