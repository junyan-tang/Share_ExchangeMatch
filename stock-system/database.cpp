#include "database.hpp"
#include "myStruct.h"

void Database::drop_table()
{
    work W(*C);
    W.exec("DROP TABLE IF EXISTS TRANSACTION CASCADE;");
    W.exec("DROP TABLE IF EXISTS BUY_ORDER CASCADE;");
    W.exec("DROP TABLE IF EXISTS SELL_ORDER CASCADE;");
    W.exec("DROP TABLE IF EXISTS SHARE CASCADE;");
    W.exec("DROP TABLE IF EXISTS ACCOUNT CASCADE;");
    W.commit();
}

void Database::create_table()
{
    string sql = "";
    sql += "CREATE TABLE ACCOUNT("
           "ACCOUNT_ID TEXT PRIMARY KEY NOT NULL,"
           "BALANCE NUMERIC NOT NULL);";

    sql += "CREATE TABLE SHARE("
           "ID SERIAL PRIMARY KEY,"
           "SHAREID TEXT NOT NULL,"
           "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
           "NUM NUMERIC NOT NULL);";

    sql += "CREATE TABLE SELL_ORDER("
           "STOCK_ID TEXT NOT NULL,"
           "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
           "NUM NUMERIC NOT NULL,"
           "PRICE NUMERIC NOT NULL,"
           "ORDER_TIME TEXT,"
           "TRANSACTION_ID TEXT NOT NULL,"
           "PRIMARY KEY (ACCOUNT_ID, STOCK_ID));";

    sql += "CREATE TABLE BUY_ORDER("
           "STOCK_ID TEXT NOT NULL,"
           "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
           "NUM NUMERIC NOT NULL,"
           "PRICE NUMERIC NOT NULL,"
           "ORDER_TIME TEXT,"
           "TRANSACTION_ID TEXT NOT NULL,"
           "PRIMARY KEY (ACCOUNT_ID, STOCK_ID));";

    sql += "CREATE TABLE TRANSACTION("
           "TRANSACTION_ID TEXT NOT NULL,"
           "TIME TEXT NOT NULL,"
           "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
           "STOCK_ID TEXT NOT NULL,"
           "NUM NUMERIC NOT NULL,"
           "PRICE NUMERIC NOT NULL,"
           "STATUS TEXT NOT NULL);";

    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::init_database()
{
    drop_table();
    create_table();
}


ResultC Database::create_account(string account_id, double balance)
{
    nontransaction N(*C);
    string sql = "SELECT * FROM ACCOUNT WHERE ACCOUNT_ID = '" + account_id + "';";
    result R(N.exec(sql));

    if (R.size() != 0)
    {
        return {account_id, "", "error", "Account already exists"};
    }

    string create_sql = "INSERT INTO ACCOUNT (ACCOUNT_ID, BALANCE) VALUES ('" + account_id + "', " + to_string(balance) + ");";
    work W(*C);
    W.exec(create_sql);
    W.commit();
    return {account_id, "", "success", ""};
}

ResultC Database::create_stock(string stock_id, string account_id, double num)
{
    work W(*C);
    string is_account_valid = "SELECT * FROM ACCOUNT WHERE ACCOUNT_ID = '" + account_id + "';";
    result R1(W.exec(is_account_valid));
    if (R1.size() == 0)
    {
        return {account_id, stock_id, "error", "Account does not exist"};
    }

    string is_stock_exist = "SELECT * FROM SHARE WHERE SHAREID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    result R2(W.exec(is_stock_exist));
    if (R2.size() == 0)
    {
        string insert_sql = "INSERT INTO SHARE (SHAREID, ACCOUNT_ID, NUM) VALUES ('" + stock_id + "', '" + account_id + "', " + to_string(num) + ");";
        W.exec(insert_sql);
        W.commit();
        return {account_id, stock_id, "success", ""};
    }
    else
    {
        double total = num + R2.begin()[3].as<double>();
        string update_sql = "UPDATE SHARE SET NUM = " + to_string(total) + " WHERE SHAREID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
        W.exec(update_sql);
        W.commit();
        return {account_id, stock_id, "success", ""};
    }
}

ResultT Database::query_order(string trans_id){
    work W(*C);
    string query_sql = "SELECT * FROM TRANSACTION WHERE TRANSACTION_ID = '" + trans_id + "';";
    result R(W.exec(query_sql));
    W.commit();
    vector<Transaction> trans_history;
    if (R.size() == 0)
    {
        return {"", "", "", "", "error", "Transaction does not exist", trans_history};
    }

    for (result::iterator i = R.begin(); i != R.end(); ++i)
        {
            Transaction curr_trans = {i[3].as<string>(), i[2].as<string>(), i[4].as<double>(), i[5].as<double>(), i[1].as<string>(), i[6].as<string>()};
            trans_history.push_back(curr_trans);
        }
    return {"", "query", trans_id, "", "success", "", trans_history};
}

ResultT Database::open_sell_order(string account_id, string sym, double amount, double limit, string trans_id){
    work W(*C);
    string inquire_stock_sql = "SELECT * FROM SHARE WHERE SHAREID = '" + sym + "' AND ACCOUNT_ID = '" + account_id + "';";
    result R1(W.exec(inquire_stock_sql));
    string timestamp = get_time();

    if (R1.size() == 0 || R1.begin()[3].as<double>() < abs(amount))
    {
        return {account_id, "order", trans_id, sym, "error", "Not enough shares", {}};
    }
    string insert_sell_order_sql = "INSERT INTO SELL_ORDER (STOCK_ID, ACCOUNT_ID, NUM, PRICE, ORDER_TIME, TRANSACTION_ID) "
                                   "VALUES ('" + sym + "', '" + account_id + "', " + to_string(abs(amount)) + ", " + to_string(limit) + ", '" + timestamp + "', , " + trans_id + ");";
    W.exec(insert_sell_order_sql);
    string update_stock_sql = "UPDATE SHARE SET NUM = " + to_string(R1.begin()[3].as<double>() - abs(amount)) + " WHERE SHAREID = '" + sym + "' AND ACCOUNT_ID = '" + account_id + "';";
    W.exec(update_stock_sql);
    string insert_transaction_sql = "INSERT INTO TRANSACTION (TRANSACTION_ID, TIME, ACCOUNT_ID, STOCK_ID, NUM, PRICE, STATUS) VALUES ('" +
                 trans_id + "', '" +
                 timestamp + "', '" +
                 account_id + "', '" +
                 sym + "', " +
                 to_string(amount) + ", " +
                 to_string(limit) + ", 'open');";
    W.exec(insert_transaction_sql);
    W.commit();
    return {account_id, "order", trans_id, sym, "success", "", {}};

}

ResultT Database::open_buy_order(string account_id, string sym, double shares, double price, string trans_id){
    work W(*C);
    string inquire_account_sql = "SELECT * FROM ACCOUNT WHERE ACCOUNT_ID = '" + account_id + "';";
    result R1(W.exec(inquire_account_sql));
    string timestamp = get_time();
    double balance = R1.begin()[1].as<double>();
    double money = shares * price;
    if (balance > money) {
        string insert_buy_order_sql = "INSERT INTO BUY_ORDER (STOCK_ID, ACCOUNT_ID, NUM, PRICE, ORDER_TIME, TRANSACTION_ID) "
                                   "VALUES ('" + sym + "', '" + account_id + "', " + to_string(shares) + ", " + to_string(price) + ", '" + timestamp + "', " + trans_id + ");";
        W.exec(insert_buy_order_sql);
        string update_account_sql = "UPDATE ACCOUNT SET BALANCE = " + to_string(balance - money) + " WHERE ACCOUNT_ID = '" + account_id + "';";
        W.exec(update_account_sql);
        string insert_transaction_sql = "INSERT INTO TRANSACTION (TRANSACTION_ID, TIME, ACCOUNT_ID, STOCK_ID, NUM, PRICE, STATUS) VALUES ('" +
                                        trans_id + "', '" +
                                        timestamp + "', '" +
                                        account_id + "', '" +
                                        sym + "', " +
                                        to_string(shares) + ", " +
                                        to_string(price) + ", 'open');";
        W.exec(insert_transaction_sql);
        W.commit();
        return {account_id, "order", trans_id, sym, "success", "", {}};
    }
    else{
        return  {account_id, "order", trans_id, sym, "error", "Not enough balance", {}};
    }

}


void Database::insert_transaction(string trans_id, string timestamp, string account_id, string stock_id, double num, double price, string status)
{
    string sql = "INSERT INTO TRANSACTION (TRANSACTION_ID, TIME, ACCOUNT_ID, STOCK_ID, NUM, PRICE, STATUS) VALUES ('" +
                 trans_id + "', '" +
                 timestamp + "', '" +
                 account_id + "', '" +
                 stock_id + "', " +
                 to_string(num) + ", " +
                 to_string(price) + ", '" +
                 status + "');";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_account(string account_id, double balance)
{
    string sql = "UPDATE ACCOUNT SET BALANCE = " + to_string(balance) + " WHERE ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_stock(string stock_id, string account_id, double num)
{
    string sql = "UPDATE SHARE SET NUM = " + to_string(num) + " WHERE SHAREID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_sell_order(string stock_id, string account_id, double num, double price, string timestamp)
{
    string sql = "UPDATE SELL_ORDER SET NUM = " + to_string(num) + ", PRICE = " + to_string(price) + ", ORDER_TIME = '" + timestamp + "' "
                "WHERE STOCK_ID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_buy_order(string stock_id, string account_id, double num, double price, string timestamp)
{
    string sql = "UPDATE BUY_ORDER SET NUM = " + to_string(num) + ", PRICE = " + to_string(price) + ", ORDER_TIME = '" + timestamp + "' "
                "WHERE STOCK_ID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_transaction(string trans_id, string account_id, string stock_id, double num, double price, string status)
{
    string sql = "UPDATE TRANSACTION SET NUM = " + to_string(num) + ", PRICE = " + to_string(price) + " WHERE TRANSACTION_ID = '" + trans_id + "';";
    cout << "this is sql: " << sql << endl;
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_transaction(string trans_id, string timestamp, string status)
{
    string sql = "UPDATE TRANSACTION SET STATUS = '" + status + "', TIME = '" + timestamp +
                "' WHERE TRANSACTION_ID = '" + trans_id + "' AND STATUS = 'open';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_transaction(string trans_id, string timestamp, string status, double price)
{
    string sql = "UPDATE TRANSACTION SET STATUS = '" + status + "', TIME = '" + timestamp +
                    "', PRICE = " + to_string(price) +
                    " WHERE TRANSACTION_ID = '" + trans_id + "' AND STATUS = 'open';";

    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_transaction(string tran_id, double num)
{
    string sql = "UPDATE TRANSACTION SET NUM = " + to_string(num) +
                    " WHERE TRANSACTION_ID = '" + tran_id + "' AND STATUS = 'open';";
    work W(*C);
    W.exec(sql);
    W.commit();

}

void Database::delete_account(string account_id)
{
    string sql = "DELETE FROM ACCOUNT WHERE ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::delete_stock(string stock_id)
{
    string sql = "DELETE FROM SHARE WHERE SHAREID = '" + stock_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}


void Database::delete_transaction(string trans_id)
{
    string sql = "DELETE FROM TRANSACTION WHERE TRANSACTION_ID = '" + trans_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::show_table(string table)
{
    string sql = "SELECT * FROM " + table + ";";
    work W(*C);
    result R = W.exec(sql);
    W.commit();
    cout << "Table: " << table << endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
    {
        cout << "row: ";
        for (auto i = c.begin(); i != c.end(); ++i)
        {
            cout << i->c_str() << " ";
        }
        cout << endl;
    }
}

ResultT Database::cancel_order(string trans_id){
    work W(*C);
    string query_sql = "SELECT * FROM TRANSACTION WHERE TRANSACTION_ID = '" + trans_id + "';";
    result R1(W.exec(query_sql));
    string timestamp = get_time();
    vector<Transaction> trans_history;

    if (R1.size() == 0){
        return {"", "cancel", trans_id, "", "error", "Transaction does not exist", {}};
    }

    string delete_buy_order_sql = "DELETE FROM BUY_ORDER WHERE TRANSACTION_ID = '" + trans_id + "';";
    W.exec(delete_buy_order_sql);
    string delete_sell_order_sql = "DELETE FROM SELL_ORDER WHERE TRANSACTION_ID = '" + trans_id + "';";
    W.exec(delete_sell_order_sql);
    string update_transaction_sql = "UPDATE TRANSACTION SET STATUS = 'canceled', TIME = '" + timestamp + "' WHERE TRANSACTION_ID = '" + trans_id + "' AND STATUS = 'open';";
    W.exec(update_transaction_sql);
    string query_transaction = "SELECT * FROM TRANSACTION WHERE TRANSACTION_ID = '" + trans_id + "' AND STATUS = 'canceled';";
    result R2(W.exec(query_transaction));
    for (result::iterator i = R2.begin(); i != R2.end(); ++i)
    {
        string account_id = i[2].as<string>();
        string stock_id = i[3].as<string>();
        double num = i[4].as<double>();
        double price = i[5].as<double>();
        string status = i[6].as<string>();
        if (num < 0)
        {
            string update_stock_sql = "UPDATE SHARE SET NUM = NUM + " + to_string(abs(num)) + " WHERE SHAREID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
            W.exec(update_stock_sql);
        }
        else
        {
            string update_account_sql = "UPDATE ACCOUNT SET BALANCE = BALANCE + " + to_string(num * price) + " WHERE ACCOUNT_ID = '" + account_id + "';";
            W.exec(update_account_sql);
        }
        Transaction curr_trans = {stock_id, account_id, num, price, timestamp, status};
        trans_history.push_back(curr_trans);
    }
    W.commit();
    return {"", "cancel", trans_id, "", "success", "", trans_history};
}

result Database::inquire_stock(string stock_id, string account_id)
{
    string sql = "SELECT * FROM SHARE WHERE SHAREID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    result R = W.exec(sql);
    W.commit();
    return R;
}


vector<Order> Database::retrieve_sell_order()
{
    string sql = "SELECT STOCK_ID, ACCOUNT_ID, NUM, PRICE, ORDER_TIME, TRANSACTION_ID FROM SELL_ORDER;";
    work W(*C);
    result R = W.exec(sql);
    W.commit();
    vector<Order> res;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
    {
        Order curr = {c[0].as<string>(), c[1].as<string>(), c[2].as<double>(), c[3].as<double>(), c[4].as<string>(), c[5].as<string>()};
        res.push_back(curr);
    }
    return res;
}
vector<Order> Database::retrieve_buy_order()
{
    string sql = "SELECT STOCK_ID, ACCOUNT_ID, NUM, PRICE, ORDER_TIME, TRANSACTION_ID FROM BUY_ORDER;";
    work W(*C);
    result R = W.exec(sql);
    W.commit();
    vector<Order> res;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c)
    {
        Order curr = {c[0].as<string>(), c[1].as<string>(), c[2].as<double>(), c[3].as<double>(), c[4].as<string>(), c[5].as<string>()};
        res.push_back(curr);
    }
    return res;
}

string Database::get_time()
{
    auto now = std::chrono::system_clock::now();
    auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    std::string timestamp = std::to_string(now_sec);
    return timestamp;
}