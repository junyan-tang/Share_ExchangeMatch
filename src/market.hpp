#include "myStruct.h"
#include <vector>

class market
{
private:
vector<Order> sell_orders;
vector<Order> buy_orders;


public:

    market();
    ~market();

    void add_sell_order(Order order);
    void add_buy_order(Order order);

    void match();

    void update_sell_orders(Order new_order);
    void update_buy_orders(Order new_order);

    void update_orders(std::vector<Order>& orders, const Order& new_order);

};

