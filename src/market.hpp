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
public:
    market(Database db): db(db){};
    ~market(){};
    Transaction match_sell();
    void update_orders();
    void sentStock(string account_id, string stock_id, double amount);
    void sentMoney(string account_id, double amount);
    map<string, vector<Order>> convertToMap(const vector<Order>& orders);
};

