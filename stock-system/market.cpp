#include "market.hpp"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <mutex>


std::mutex mtx2;

void market::print_orders()
{
    for (auto &sell_pair : sell_orders)
    {
        cout << "stock_id: " << sell_pair.first << endl;
        for (auto &order : sell_pair.second)
        {
            cout << " stock_id: " << order.stock_id << " account_id: " << order.account_id << " num: " << order.num << " price: " << order.price << " order_time: " << order.order_time << endl;
        }
    }
    for (auto &buy_pair : buy_orders)
    {
        cout << "stock_id: " << buy_pair.first << endl;
        for (auto &order : buy_pair.second)
        {
            cout << " stock_id: " << order.stock_id << " account_id: " << order.account_id << " num: " << order.num << " price: " << order.price << " order_time: " << order.order_time << endl;
        }
    }
}

void market::update_orders()
{
    sell_orders = convertToMap(db.retrieve_sell_order());
    buy_orders = convertToMap(db.retrieve_buy_order());
}

map<string, vector<Order>> market::convertToMap(const vector<Order> &orders)
{
    map<string, vector<Order>> order_map;
    for (const auto &order : orders)
    {
        order_map[order.stock_id].push_back(order);
    }
    return order_map;
}

Transaction market::match_sell()
{
    std::lock_guard<std::mutex> lck (mtx2);
    update_orders();
    Transaction transaction;

    for (auto &sell_pair : sell_orders)
    {
        auto &stock_sell_orders = sell_pair.second;
        auto sell_it = stock_sell_orders.begin();
        while (sell_it != stock_sell_orders.end())
        {
            vector<Order> &waitlist = buy_orders[sell_it->stock_id];
            sort(waitlist.begin(), waitlist.end(), [](const Order &a, const Order &b)
                 { return a.order_time < b.order_time; });

            auto buy_it = waitlist.begin();
            while (buy_it != waitlist.end() && sell_it->num > 0)
            {

                if (buy_it->price >= sell_it->price)
                {
                    int match_quantity = min(buy_it->num, sell_it->num);
                    double match_price;
                    if (stoi(sell_it->order_time) < stoi(buy_it->order_time))
                    {
                        match_price = sell_it->price;
                    }
                    else
                    {
                        match_price = buy_it->price;
                    }
                    sentStock(buy_it->account_id, buy_it->stock_id, match_quantity);
                    sentMoney(sell_it->account_id, match_quantity * match_price);

                    sell_it->num -= match_quantity;
                    buy_it->num -= match_quantity;

                    if (buy_it->num <= 0)
                    {
                        string timestamp = get_time();
                        db.update_transaction(buy_it->transaction_id, timestamp, "executed", match_price);
                        buy_it = waitlist.erase(buy_it);
                    }
                    else
                    {
                        string timestamp = get_time();
                        db.update_transaction(buy_it->transaction_id, buy_it->num);
                        db.insert_transaction(buy_it->transaction_id, timestamp, buy_it->account_id, buy_it->stock_id, match_quantity, match_price, "executed");
                        db.update_buy_order(buy_it->stock_id, buy_it->order_time, buy_it->num, buy_it->price, buy_it->account_id);

                        ++buy_it;
                    }
                    if (sell_it->num <= 0)
                    {
                        string timestamp = get_time();
                        db.update_transaction(sell_it->transaction_id, timestamp, "executed", match_price);
                        sell_it = stock_sell_orders.erase(sell_it);

                        break;
                    }
                    else
                    {
                        string timestamp = get_time();
                        db.update_transaction(sell_it->transaction_id, -sell_it->num);
                        db.insert_transaction(sell_it->transaction_id, timestamp, sell_it->account_id, sell_it->stock_id, -match_quantity, match_price, "executed");
                        db.update_sell_order(sell_it->stock_id, sell_it->order_time, sell_it->num, sell_it->price, sell_it->account_id);
                    }
                }
                else
                {
                    ++buy_it;
                }
            }

            if (sell_it != stock_sell_orders.end())
            {
                ++sell_it;
            }
        }
    }

    return transaction;
}
void market::sentStock(string account_id, string stock_id, double amount) {
    try {
        work W(*C); // Start a transaction

        // Lock the stock row for update to prevent concurrent modifications
        result res = W.exec("SELECT NUM FROM SHARE WHERE SHAREID = " + W.quote(stock_id) + " AND ACCOUNT_ID = " + W.quote(account_id) + " FOR UPDATE");

        double new_amount;
        if (res.size() > 0) {
            // If the stock exists, update the existing amount
            double curr_shares = res[0][0].as<double>();
            new_amount = curr_shares + amount;
        } else {
            // If the stock does not exist, we might want to create it with the new amount
            new_amount = amount;
            // Depending on your business logic, you might want to handle this differently
        }

        // Update the stock amount
        W.exec("UPDATE SHARE SET NUM = " + W.quote(new_amount) + " WHERE SHAREID = " + W.quote(stock_id) + " AND ACCOUNT_ID = " + W.quote(account_id));

        W.commit(); // Commit the transaction
    } catch (const std::exception &e) {
        cerr << "Exception in sentStock: " << e.what() << endl;
        // Handle the exception, rollback, or propagate the error as needed
    }
}

void market::sentMoney(string account_id, double amount) {
    try {
        work W(*C); // Start a transaction

        // Lock the account row for update to prevent concurrent modifications
        result res = W.exec("SELECT BALANCE FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(account_id) + " FOR UPDATE");

        if (res.size() > 0) {
            // If the account exists, update the balance
            double balance = res[0][0].as<double>();
            double new_balance = balance + amount;

            // Update the account balance
            W.exec("UPDATE ACCOUNT SET BALANCE = " + W.quote(new_balance) + " WHERE ACCOUNT_ID = " + W.quote(account_id));
        } else {
            // Handle the situation where the account does not exist
            // Depending on your business logic, you might want to create a new account or handle it as an error
        }

        W.commit(); // Commit the transaction
    } catch (const std::exception &e) {
        cerr << "Exception in sentMoney: " << e.what() << endl;
        // Handle the exception, rollback, or propagate the error as needed
    }
}

string market::get_time()
{
    auto now = std::chrono::system_clock::now();
    auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    std::string timestamp = std::to_string(now_sec);
    return timestamp;
}
