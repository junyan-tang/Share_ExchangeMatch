

#include "market.hpp"

void market::add_sell_order(Order order){
    sell_orders.push_back(order);
}

void market::add_buy_order(Order order){
    buy_orders.push_back(order);
}

void market::match(){

}

void market::update_sell_orders(Order new_order) {
    update_orders(sell_orders, new_order);
}

void market::update_buy_orders(Order new_order) {
    update_orders(buy_orders, new_order);
}

void market::update_orders(std::vector<Order>& orders, const Order& new_order) {
    for (auto &order : orders) {
        if (order.stock_id == new_order.stock_id && order.account_id == new_order.account_id) {
            order.num = new_order.num;
            order.price = new_order.price;
            break; 
        }
    }
}


