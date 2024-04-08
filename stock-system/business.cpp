#include "business.hpp"
#include <chrono>
#include <ctime>
#include <mutex>
std::mutex mtx;

ResultC Creation::createAccount(string account_id, string balance) {
    result R;
    work W(*C);
    try {
        W.exec("BEGIN;");
        R = W.exec("SELECT ACCOUNT_ID FROM ACCOUNT WHERE ACCOUNT_ID=" + W.quote(account_id));
        if (R.size() != 0) {
            W.exec("ROLLBACK;");
            return {account_id, "", "error", "Account already exists"};
        }
        double curr_balance = stod(balance);
        if (curr_balance >= 0) {
            W.exec("INSERT INTO ACCOUNT (ACCOUNT_ID, BALANCE) VALUES (" + W.quote(account_id) + ", " + W.quote(balance) + ")");
            W.exec("COMMIT;");
            return {account_id, "", "success", ""};
        } else {
            W.exec("ROLLBACK;");
            return {account_id, "", "error", "Balance cannot be negative"};
        }
    } catch (const std::exception &e) {
        W.exec("ROLLBACK;");
        cerr << e.what() << endl;
        return {account_id, "", "error", "Database error"};
    }
}

ResultC Creation::createStock(string sym, string account_id, double amount) {

    std::lock_guard<std::mutex> lck(mtx);
    ResultC res;
    
    try {
        work W(*C);
        
 
        result account = W.exec("SELECT * FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(account_id));
        if (account.size() == 0) {
            res = {account_id, sym, "error", "Account does not exist"};
            W.commit(); 
            return res;
        }
        
  
        result R = W.exec("SELECT * FROM SHARE WHERE SHAREID = " + W.quote(sym) + " AND ACCOUNT_ID = " + W.quote(account_id) + " FOR UPDATE");
        if (R.size() == 0) {
    
            W.exec("INSERT INTO SHARE (SHAREID, ACCOUNT_ID, NUM) VALUES (" + W.quote(sym) + ", " + W.quote(account_id) + ", " + W.quote(amount) + ")");
        } else {
     
            double current_amount = R.begin()[2].as<double>(); 
            double total = current_amount + amount;
            W.exec("UPDATE SHARE SET NUM = " + W.quote(total) + " WHERE SHAREID = " + W.quote(sym) + " AND ACCOUNT_ID = " + W.quote(account_id));
        }
        

        W.commit();
        res = {account_id, sym, "success", ""};
    } catch (const std::exception &e) {

        cerr << "Exception: " << e.what() << std::endl;
        res = {account_id, sym, "error", "Database operation failed"};
    }

    return res;
}

ResultT Transact::openOrder(string account_id, string sym, string amount, string limit, string trans_id) {
    cout << "Here is open order" << endl;

    ResultT res;
    vector<Transaction> trans_history;
    double shares = stod(amount);
    double price = stod(limit);
    std::string timestamp = mkt.get_time();
    Transaction curr = {sym, account_id, shares, price, timestamp, ""};
    trans_history.push_back(curr);

    try {
        work W(*C); // 开始一个新事务
    
        if (shares < 0) { // 处理卖单
            result R = W.exec("SELECT * FROM SHARE WHERE SHAREID = " + W.quote(sym) + " AND ACCOUNT_ID = " + W.quote(account_id) + " FOR UPDATE");
            double curr_shares = R.size() == 0 ? 0 : R.begin()[2].as<double>();

            if (curr_shares >= abs(shares)) {

                db.insert_sell_order(W, sym, account_id, abs(shares), price, timestamp, trans_id);
                db.update_stock(W, sym, account_id, curr_shares + shares); // 这里应该是减去卖出的股票数量
                db.insert_transaction(W, trans_id, timestamp, account_id, sym, shares, price, "open");
                res = {account_id, "order", trans_id, sym, "success", "", trans_history};
            } else {
                W.abort(); // 没有足够的股票，取消事务
                res = {account_id, "order", trans_id, sym, "error", "Not enough shares", trans_history};
                return res;
            }
        } else if (shares > 0) { // 处理买单
            result R = W.exec("SELECT * FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(account_id) + " FOR UPDATE");
            double balance = R.begin()[1].as<double>();
            double money_needed = shares * price;
            cout << "money_needed: " << money_needed << endl;
            if (balance >= money_needed) {
                db.insert_buy_order(W, sym, account_id, shares, price, timestamp, trans_id);
                db.update_account(W, account_id, balance - money_needed);
                db.insert_transaction(W, trans_id, timestamp, account_id, sym, shares, price, "open");
                res = {account_id, "order", trans_id, sym, "success", "", trans_history};
            } else {
                W.abort(); // 余额不足，取消事务
                res = {account_id, "order", trans_id, sym, "error", "Not enough balance", trans_history};
                return res;
            }
        }

        W.commit(); // 提交事务
    } catch (const std::exception& e) {
        cerr << "Transaction failed: " << e.what() << endl;
        // 这里可能需要根据异常类型决定是否回滚事务
        res = {account_id, "order", trans_id, sym, "error", "Transaction failed", trans_history};
    }
    cout << "start match sell now" << endl;
    mkt.match_sell(); // 这个方法需要在事务外执行，并且也需要处理并发问题
    return res;
}

ResultT Transact::cancelOrder(string trans_id) {
    ResultT res;
    vector<Transaction> trans_history;
    std::string timestamp = mkt.get_time();

    try {
        work W(*C);  // Start a transaction

        // Lock the transaction row for update to prevent concurrent modifications
        result R = W.exec("SELECT * FROM TRANSACTION WHERE TRANSACTION_ID = " + W.quote(trans_id) + " FOR UPDATE");
        
        if (R.size() != 0) {
            string status = R.begin()[6].as<string>();
            if (status != "open") {
                res = {"", "cancel", trans_id, "", "error", "Transaction is not open", trans_history};
                W.commit();  // Commit to end the transaction
                return res;
            }

            // Cancel sell and buy orders within the same transaction to maintain consistency

       
            W.exec("DELETE FROM SELL_ORDER WHERE TRANSACTION_ID = '" + trans_id + "';");
            W.exec("DELETE FROM BUY_ORDER WHERE TRANSACTION_ID = '" + trans_id + "';");
            W.exec("UPDATE TRANSACTION SET STATUS = '" + status + "', TIME = '" + timestamp +
                "' WHERE TRANSACTION_ID = '" + trans_id + "' AND STATUS = 'open';");
            
    

            // Push the canceled transaction into the history
            Transaction curr_trans = {
                R.begin()[3].as<string>(),   // stock_id
                R.begin()[2].as<string>(),   // account_id
                R.begin()[4].as<double>(),   // num
                R.begin()[5].as<double>(),   // price
                timestamp,                   // updated timestamp
                "canceled"                   // new status
            };
            trans_history.push_back(curr_trans);
            
            res = {"", "cancel", trans_id, "", "success", "", trans_history};
        } else {
            res = {"", "cancel", trans_id, "", "error", "Transaction does not exist", trans_history};
        }

        W.commit();  // Commit the transaction
    } catch (const std::exception &e) {
        cerr << "Exception in cancelOrder: " << e.what() << endl;
        res = {"", "cancel", trans_id, "", "error", "Database operation failed", trans_history};
    }

    return res;
}

ResultT Transact::queryOrder(string trans_id) {
    ResultT res;
    vector<Transaction> trans_history;

    try {
        nontransaction N(*C); 
        result R = N.exec("SELECT * FROM TRANSACTION WHERE TRANSACTION_ID = " + N.quote(trans_id));

        if (R.size() != 0) {
            for (const auto &row : R) {
                Transaction curr_trans = {row[3].as<string>(), row[2].as<string>(), row[4].as<double>(), row[5].as<double>(), row[1].as<string>(), row[6].as<string>()};
                trans_history.push_back(curr_trans);
            }
            res = {"", "query", trans_id, "", "success", "", trans_history};
        } else {
            res = {"", "query", trans_id, "", "error", "Transaction does not exist", trans_history};
        }
    } catch (const std::exception &e) {
        cerr << "Exception in queryOrder: " << e.what() << endl;
        res = {"", "query", trans_id, "", "error", "Database operation failed", trans_history};
    }

    return res;
}


bool Transact::checkAccount(string account_id) {
    try {

        work W(*C);
        result R = W.exec("SELECT ACCOUNT_ID FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(account_id));
        W.commit();
        return R.size() != 0;
    } catch (const std::exception &e) {
        cerr << "Exception in checkAccount: " << e.what() << endl;
   
        return false;
    }
}
