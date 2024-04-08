#include "business.hpp"
#include <chrono>
#include <ctime>

ResultC Creation::createAccount(string account_id, string balance)
{
    result R = db.inquire_account(account_id);
    ResultC res;
    double curr_balance = stod(balance);
    if (R.size() == 0 and curr_balance >= 0)
    {
        db.insert_account(account_id, curr_balance);
        res = {account_id, "", "success", ""};
    }
    else if (curr_balance < 0)
    {
        res = {account_id, "", "error", "Balance cannot be negative"};
    }
    else
    {
        res = {account_id, "", "error", "Account already exists"};
    }
    return res;
}

ResultC Creation::createStock(string sym, string account_id, double amount)
{
    result account = db.inquire_account(account_id);
    ResultC res;
    if (account.size() == 0)
    {
        res = {account_id, sym, "error", "Account does not exist"};
    }
    else
    {
        result R = db.inquire_stock(sym, account_id);
        if (R.size() == 0)
        {
            db.insert_stock(sym, account_id, amount);
            res = {account_id, sym, "success", ""};
        }
        else
        {
            double total = amount + R.begin()[3].as<double>();
            db.update_stock(sym, account_id, total);
            res = {account_id, sym, "success", ""};
        }
    }
    return res;
}

ResultT Transact::openOrder(string account_id, string sym, string amount, string limit, string trans_id)
{
    ResultT res;
    vector<Transaction> trans_history;
    double shares = stod(amount);
    double price = stod(limit);
    std::string timestamp = mkt.get_time();
    Transaction curr = {sym, account_id, shares, price, timestamp, ""};
    trans_history.push_back(curr);
    if (shares < 0)
    {
        result R = db.inquire_stock(sym, account_id);
        double curr_shares;
        if (R.size() == 0)
        {
            curr_shares = 0;
        }
        else
        {
            curr_shares = R.begin()[3].as<double>();
        }
        if (curr_shares >= abs(shares))
        {
            db.insert_sell_order(sym, account_id, abs(shares), price, timestamp, trans_id);
            db.update_stock(sym, account_id, curr_shares + shares);
            db.insert_transaction(trans_id, timestamp, account_id, sym, shares, price, "open");
            res = {account_id, "order", trans_id, sym, "success", "", trans_history};
        }
        else
        {
            res = {account_id, "order", trans_id, sym, "error", "Not enough shares", trans_history};
        }
    }
    else if (shares > 0)
    {
        result R = db.inquire_account(account_id);
        double balance = R.begin()[1].as<double>();
        double money = shares * price;
        if (balance > money)
        {
            db.insert_buy_order(sym, account_id, shares, price, timestamp, trans_id);
            db.update_account(account_id, balance - money);
            db.insert_transaction(trans_id, timestamp, account_id, sym, shares, price, "open");
            res = {account_id, "order", trans_id, sym, "success", "", trans_history};
        }
        else
        {
            res = {account_id, "order", trans_id, sym, "error", "Not enough balance", trans_history};
        }
    }
    mkt.match_sell();
    return res;
}

ResultT Transact::cancelOrder(string trans_id)
{
    result R = db.inquire_transaction(trans_id);
    ResultT res;
    vector<Transaction> trans_history;
    std::string timestamp = mkt.get_time();
    if (R.size() != 0)
    {
        db.delete_buy_order(trans_id);
        db.delete_sell_order(trans_id);
        db.update_transaction(trans_id, timestamp, "canceled");
        R = db.inquire_transaction(trans_id);
        for (result::iterator i = R.begin(); i != R.end(); ++i)
        {
            string trans_id = i[0].as<string>();
            string timestamp = i[1].as<string>();
            string account_id = i[2].as<string>();
            string stock_id = i[3].as<string>();
            double num = i[4].as<double>();
            double price = i[5].as<double>();
            string status = i[6].as<string>();
            if (status == "canceled")
            {
                if (num < 0)
                {
                    sentStock(account_id, stock_id, abs(num));
                }
                else
                {
                    sentMoney(account_id, num * price);
                }
            }
            Transaction curr_trans = {stock_id, account_id, num, price, timestamp, status};
            trans_history.push_back(curr_trans);
        }
        res = {"", "cancel", trans_id, "", "success", "", trans_history};
    }
    else
    {
        res = {"", "cancel", trans_id, "", "error", "Transaction does not exist", trans_history};
    }
    return res;
}

ResultT Transact::queryOrder(string trans_id)
{
    result R = db.inquire_transaction(trans_id);
    ResultT res;
    vector<Transaction> trans_history;
    if (R.size() != 0)
    {
        for (result::iterator i = R.begin(); i != R.end(); ++i)
        {
            Transaction curr_trans = {i[3].as<string>(), i[2].as<string>(), i[4].as<double>(), i[5].as<double>(), i[1].as<string>(), i[6].as<string>()};
            trans_history.push_back(curr_trans);
        }
        res = {"", "query", trans_id, "", "success", "", trans_history};
    }
    else
    {
        res = {"", "query", trans_id, "", "error", "Transaction does not exist", trans_history};
    }
    return res;
}

bool Transact::checkAccount(string account_id)
{
    result R = db.inquire_account(account_id);
    if (R.size() == 0)
    {
        return false;
    }
    return true;
}