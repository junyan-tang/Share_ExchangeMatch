#include "myStruct.h"
#include <vector>
#include <map>
#include "database.hpp"
using namespace std;
class market
{
private:
    map<string, vector<Order>> sell_orders;
    map<string, vector<Order>> buy_orders;

    Database db;
public:

    market(Database db): db(db){};
    ~market();

    void add_sell_order(Order order);
    void add_buy_order(Order order);

    Transaction match_sell();

    void update_sell_orders(Order new_order);
    void update_buy_orders(Order new_order);

    void update_orders();

    map<string, vector<Order>> convertToMap(const vector<Order>& orders);


};

