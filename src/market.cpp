#include "market.hpp"
#include <algorithm>

// void market::add_sell_order(Order order){
//     sell_orders[order.stock_id].push_back(order);
// }

// void market::add_buy_order(Order order){
//     buy_orders[order.stock_id].push_back(order);
// }



// void market::update_sell_orders(Order new_order) {
//     update_orders(sell_orders, new_order);
// }

// void market::update_buy_orders(Order new_order) {
//     update_orders(buy_orders, new_order);
// }


void market::update_orders(){

    sell_orders = convertToMap(db.retrieve_sell_order());
    buy_orders = convertToMap(db.retrieve_buy_order());
    cout << "this is sell_orders" << endl;
    for(auto& sell_pair : sell_orders){
        cout << "stock_id: " << sell_pair.first << endl;
        for(auto& order : sell_pair.second){
            cout << " stock_id: " << order.stock_id << " account_id: " << order.account_id << " num: " << order.num << " price: " << order.price << " order_time: " << order.order_time << endl;
        }
    }
    cout << "\n=====================" << endl;
    cout << "this is buy_orders" << endl;
    for(auto& buy_pair : buy_orders){
        cout << "stock_id: " << buy_pair.first << endl;
        for(auto& order : buy_pair.second){
            cout << " stock_id: " << order.stock_id << " account_id: " << order.account_id << " num: " << order.num << " price: " << order.price << " order_time: " << order.order_time << endl;
        }
    }
        cout << "\n=====================" << endl;
}

map<string, vector<Order>> market::convertToMap(const vector<Order>& orders) {
    map<string, vector<Order>> order_map;
    for (const auto& order : orders) {
        order_map[order.stock_id].push_back(order);
    }
    return order_map;
}


Transaction market::match_sell() {

    update_orders();
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
                    cout << "this is transactino ====" << endl;
                    cout << "transaction: " << transaction.stock_id << " " << transaction.account_id << " " << transaction.num << " " << transaction.price << " " << transaction.order_time << " " << transaction.status << endl;
                    cout << "\n ======= " << endl;

                    //update order
                    sell_it->num -= match_quantity;
                    buy_it->num -= match_quantity;

                    //delete buy order if num < 0
                    if (buy_it->num <= 0) {
                        buy_it = waitlist.erase(buy_it);
                    } else {
                        ++buy_it;
                    }


                    //update database
                    //update sell order
                    //db.update_transaction(sell_it->order_id, sell_it->order_time, "closed");
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


market::~market() {
    // Empty body
}
