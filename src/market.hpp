#include "myStruct.h"
#include <vector>
#include <map>
using namespace std;
class market
{
private:
    map<string, vector<Order>> sell_orders;
    map<string, vector<Order>> buy_orders;


public:

    market();
    ~market();

    void add_sell_order(Order order);
    void add_buy_order(Order order);

    Transaction match_sell();

    void update_sell_orders(Order new_order);
    void update_buy_orders(Order new_order);

    void update_orders(map<string, vector<Order>> & orders, const Order& new_order);

};

