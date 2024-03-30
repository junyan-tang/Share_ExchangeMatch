#include "business.hpp"
#include <chrono>
#include <ctime>
int transaction_id = 0;
ResultC Creation::createAccount(string account_id, string balance){
    result R = db.inquire_account(account_id);
    ResultC res;
    double curr_balance = stod(balance);
    if(R.size() == 0){
        db.insert_account(account_id, curr_balance);
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

ResultT Transact::openOrder(string account_id, string sym, string amount, string limit, int trans_id){
    ResultT res;
    vector<Transaction> trans_history;
    double shares = stod(amount);
    double price = stod(limit);
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    string timestamp = ctime(&now_c);
    if (shares < 0){
        result R = db.inquire_stock(sym, account_id);
        double curr_shares = R.begin()[2].as<double>();
        if(curr_shares >= abs(shares)){
            db.insert_sell_order(sym, account_id, abs(shares), price, timestamp);
            db.update_stock(sym, account_id, curr_shares + shares);
            res = {account_id, "order", trans_id, sym, "success", "", trans_history};
        }
        else{
            res = {account_id, "order", trans_id, sym, "error", "Not enough shares", trans_history};
        }
    }
    else if (shares > 0){
        result R = db.inquire_account(account_id);
        double balance = R.begin()[1].as<double>();
        double money = shares * price;
        if (balance > money) {
            db.insert_buy_order(sym, account_id, shares, price, timestamp);
            db.update_account(account_id, balance - money);
            res = {account_id, "order", trans_id, sym, "success", "", trans_history};
        }
        else{
            res = {account_id, "order", trans_id, sym, "error", "Not enough balance", trans_history};
        }
    }

    return res;
}

ResultT Transact::cancelOrder(int trans_id){
    result R = db.inquire_transaction(trans_id);
    ResultT res;
    vector<Transaction> trans_history;
    if (R.size() != 0){
        db.update_transaction(trans_id, "canceled");
        R = db.inquire_transaction(trans_id);
        for (result::iterator i = R.begin(); i != R.end(); ++i){
            Transaction curr_trans = {i[0].as<string>(), i[1].as<string>(), i[2].as<int>(), i[3].as<double>(), i[4].as<string>(), "canceled"};
            trans_history.push_back(curr_trans);
        }
        res = {"", "cancel", trans_id, "", "success", "", trans_history};
    }
    else {
        res = {"", "cancel", trans_id, "", "error", "Transaction does not exist", trans_history};
    }
    return res;
}

ResultT Transact::queryOrder(int trans_id){
    result R = db.inquire_transaction(trans_id);
    ResultT res;
    vector<Transaction> trans_history;
    if (R.size() != 0){
        for (result::iterator i = R.begin(); i != R.end(); ++i){
            Transaction curr_trans = {i[0].as<string>(), i[1].as<string>(), i[2].as<int>(), i[3].as<double>(), i[4].as<string>(), i[5].as<string>()};
            trans_history.push_back(curr_trans);
        }
        res = {"", "query", trans_id, "", "success", "", trans_history};
    }
    else {
        res = {"", "query", trans_id, "", "error", "Transaction does not exist", trans_history};
    }
    return res;
}