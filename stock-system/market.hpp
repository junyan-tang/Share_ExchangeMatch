#include "myStruct.h"
#include <vector>
#include <map>
#include "database.hpp"
using namespace std;
class market {
private:
    map<string, vector<Order>> sell_orders;
    map<string, vector<Order>> buy_orders;
    Database db;
    connection * C = root_connection;
    
public:
    market(Database db): db(db){};
    ~market(){};
    Transaction match_sell();
    void update_orders();
    void sentStock(work & W, string account_id, string stock_id, double amount);
    void sentMoney(work & W, string account_id, double amount);
    map<string, vector<Order>> convertToMap(const vector<Order>& orders);
    string get_time();
    void print_orders();

    void update_transaction(work & W, string tran_id, double num);
    void update_transaction(work & W, string trans_id, string timestamp, string status, double price);
    void insert_transaction(work & W, string trans_id, string timestamp, string account_id, string stock_id, double num, double price, string status);

    void update_buy_order(work & W, string stock_id, string account_id, double num, double price, string timestamp);
    void update_sell_order(work & W, string stock_id, string account_id, double num, double price, string timestamp);

    
};

