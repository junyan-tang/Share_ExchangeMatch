#include "business.hpp"
#include <chrono>
#include <ctime>

ResultC Creation::createAccount(string account_id, string balance)
{
    double curr_balance = stod(balance);
    if (curr_balance < 0)
    {
        return {account_id, "", "error", "Balance cannot be negative"};
    }
    
    return db.create_account(account_id, curr_balance);
}

ResultC Creation::createStock(string sym, string account_id, double amount)
{
    return db.create_stock(sym, account_id, amount);
}

ResultT Transact::openOrder(string account_id, string sym, string amount, string limit, string trans_id)
{   
    double shares = stod(amount);
    double price = stod(limit);
    string timestamp = db.get_time();
    Transaction curr = {sym, account_id, shares, price, timestamp, ""};
    vector<Transaction> trans_history;
    trans_history.push_back(curr);
    ResultT res;
    if (shares < 0)
    {
        res = db.open_sell_order(account_id, sym, shares, price, trans_id);
    }
    else if (shares > 0)
    {
        res = db.open_sell_order(account_id, sym, shares, price, trans_id);
    }
    res.transaction = trans_history;
    mkt.match_sell();
    return res;
}

ResultT Transact::cancelOrder(string trans_id)
{
    return db.cancel_order(trans_id);
}

ResultT Transact::queryOrder(string trans_id)
{
    return db.query_order(trans_id);
}