#include "business.hpp"
#include <chrono>
#include <ctime>

ResultC Creation::createAccount(string account_id, string balance){
    result R = db.inquire_account(account_id);
    ResultC res;
    if(R.size() == 0){
        db.insert_account(account_id, balance);
        res = {account_id, "", "success", ""};
    }
    else{
        res = {account_id, "", "error", "Account already exists"};
    }
    return res;
}

ResultC Creation::createStock(string sym, string account_id, double amount){
    result account = db.inquire_account(account_id);
    ResultC res;
    if(account.size() == 0){
        res = {account_id, sym, "error", "Account does not exist"};
    }
    else{
        result R = db.inquire_stock(sym, account_id);
        if(R.size() == 0){
            db.insert_stock(sym, account_id, amount);
            res = {account_id, sym, "success", ""};
        }
        else{
            double total = amount + R.begin()[0].as<double>();
            db.update_stock(sym, account_id, total);
            res = {account_id, sym, "success", ""};
        }
    }
    return res;
}

void Transact::openOrder(string account_id, string stock_id, string amount, string limit){
    double shares = stod(amount);
    double price = stod(limit);
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    string timestamp = ctime(&now_c);
    if (shares < 0){
        db.insert_sell_order(stock_id, account_id, abs(shares), price, timestamp);
    }
    else if (shares > 0){
        db.insert_buy_order(stock_id, account_id, shares, price, timestamp);
    }
}

void Transact::cancelOrder(string transaction_id){
    db.delete_transaction(transaction_id);
}

void Transact::queryOrder(string transaction_id){
    
}

bool Transact::checkAccount(string account_id){
    result R = db.inquire_account(account_id);
    if(R.size() == 0){
        return false;
    }
    return true;
}