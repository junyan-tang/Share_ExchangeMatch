

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



Transaction market::match_sell() {
    Transaction transaction;
    for (auto& sell_pair : sell_orders) {
        auto& stock_sell_orders = sell_pair.second;

        auto sell_it = stock_sell_orders.begin();
        while (sell_it != stock_sell_orders.end()) {
            vector<Order>& waitlist = buy_orders[sell_it->stock_id];
            sort(waitlist.begin(), waitlist.end(), [](const Order& a, const Order& b) {
                return a.order_time < b.order_time;
            });

            auto buy_it = waitlist.begin();
            while (buy_it != waitlist.end() && sell_it->num > 0) {
                if (buy_it->price >= sell_it->price) {
                    int match_quantity = min(buy_it->num, sell_it->num);

                    //create transaction
                    transaction.stock_id = sell_it->stock_id;
                    transaction.account_id = buy_it->account_id;
                    transaction.num = match_quantity;
                    transaction.price = sell_it->price;
                    transaction.order_time = sell_it->order_time;
                    transaction.status = "sell";
                    

                    //update order
                    sell_it->num -= match_quantity;
                    buy_it->num -= match_quantity;

                    //delete buy order if num < 0
                    if (buy_it->num <= 0) {
                        buy_it = waitlist.erase(buy_it);
                    } else {
                        ++buy_it;
                    }
                } else {
                    ++buy_it;
                }
            }

            //delete sell order if num < 0
            if (sell_it->num <= 0) {
                sell_it = stock_sell_orders.erase(sell_it);
            } else {
                ++sell_it;
            }
        }
    }

    return transaction;
}
