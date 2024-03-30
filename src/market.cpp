

#include "market.hpp"
#include <algorithm>

void market::add_sell_order(Order order){
    sell_orders[order.stock_id].push_back(order);
}

void market::add_buy_order(Order order){
    buy_orders[order.stock_id].push_back(order);
}



void market::update_sell_orders(Order new_order) {
    update_orders(sell_orders, new_order);
}

void market::update_buy_orders(Order new_order) {
    update_orders(buy_orders, new_order);
}

void market::update_orders(map<string, vector<Order>> & orders, const Order& new_order) {
    for (auto &order : orders[new_order.stock_id]) {
        if (order.stock_id == new_order.stock_id && order.account_id == new_order.account_id) {
            order.num = new_order.num;
            order.price = new_order.price;
            break; 
        }
    }
}



void market::match_sell(Order sell_order){
    vector<Order> &  waitlist = buy_orders[sell_order.stock_id];
    sort(waitlist.begin(), waitlist.end(), [](const Order& a, const Order& b) {
        return a.order_time < b.order_time; 
    });

    for (Order & candidate : waitlist){

    }

    
}