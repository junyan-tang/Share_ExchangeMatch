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

void Database::insert_account(string account_id, double balance)
{
    string sql = "INSERT INTO ACCOUNT (ACCOUNT_ID, BALANCE) VALUES ('" + account_id + "', " + to_string(balance) + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::insert_stock(string stock_id, string account_id, double num)
{
    string sql = "INSERT INTO SHARE (SHAREID, ACCOUNT_ID, NUM) VALUES ('" + stock_id + "', '" + account_id + "', " + to_string(num) + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::insert_sell_order(string stock_id, string account_id, double num, double price, string timestamp, string trans_id)
{
    string sql = "INSERT INTO SELL_ORDER (STOCK_ID, ACCOUNT_ID, NUM, PRICE, ORDER_TIME, TRANSACTION_ID) "
                 "VALUES ('" +
                 stock_id + "', '" + account_id + "', " + to_string(num) + ", " + to_string(price) + ", '" + timestamp + "', " + trans_id + ");";
    work W(*C);
    W.exec(sql);
    W.commit();

}

void Database::insert_buy_order(string stock_id, string account_id, double num, double price, string timestamp, string trans_id)
{
    string sql = "INSERT INTO BUY_ORDER (STOCK_ID, ACCOUNT_ID, NUM, PRICE, ORDER_TIME, TRANSACTION_ID) "
                 "VALUES ('" +
                 stock_id + "', '" + account_id + "', " + to_string(num) + ", " + to_string(price) + ", '" + timestamp + "', " + trans_id + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
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

void Database::delete_sell_order(string trans_id)
{
    string sql = "DELETE FROM SELL_ORDER WHERE TRANSACTION_ID = '" + trans_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::delete_buy_order(string trans_id)
{
    string sql = "DELETE FROM BUY_ORDER WHERE TRANSACTION_ID = '" + trans_id + "';";
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

result Database::inquire_account(string account_id)
{
    string sql = "SELECT * FROM ACCOUNT WHERE ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    pqxx::result R = W.exec(sql);
    W.commit();
    return R;
}

result Database::inquire_stock(string stock_id, string account_id)
{
    string sql = "SELECT * FROM SHARE WHERE SHAREID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    result R = W.exec(sql);
    W.commit();
    return R;
}

result Database::inquire_transaction(string trans_id)
{
    string sql = "SELECT * FROM TRANSACTION WHERE TRANSACTION_ID = '" + trans_id + "';";
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