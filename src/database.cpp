


#include <iostream>
#include "database.hpp"

void Database::drop_table(connection *C){
    cout << "Dropping table" << endl;

    //drop User, Share, Order
    string sql = "DROP TABLE IF EXISTS User; DROP TABLE IF EXISTS SHARE; DROP TABLE IF EXISTS Order;";

    work W(*C);
    W.exec(sql);
    W.commit();
    


}

void Database::create_table(connection *C){
    cout << "Creating table" << endl;
    string sql ="";
    sql +=  "CREATE TABLE ACCOUNT("
            "ACCOUNT_ID TEXT PRIMARY KEY NOT NULL,"
            "BALANCE NUMERIC NOT NULL);";

    sql +=  "CREATE TABLE SHARE("
            "SHAREID TEXT PRIMARY KEY NOT NULL,"
            "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
            "NUM NUMERIC NOT NULL);";

    sql += "CREATE TABLE SELL_ORDER("
            "SHARE_ID TEXT REFERENCES SHARE(SHAREID) NOT NULL,"
            "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
            "NUM NUMERIC NOT NULL,"
            "PRICE NUMERIC NOT NULL,"
            "PRIMARY KEY (ACCOUNT_ID, SHARE_ID));";

    sql += "CREATE TABLE BUY_ORDER("
            "SHARE_ID TEXT REFERENCES SHARE(SHAREID) NOT NULL,"
            "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
            "NUM NUMERIC NOT NULL,"
            "PRICE NUMERIC NOT NULL,"
            "PRIMARY KEY (ACCOUNT_ID, SHARE_ID));";

    sql += "CREATE TABLE TRANSACTION("
            "TRANSACTION_ID TEXT PRIMARY KEY NOT NULL,"
            "ACCOUNT_ID TEXT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
            "SHARE_ID TEXT REFERENCES SHARE(SHAREID) NOT NULL,"
            "NUM NUMERIC NOT NULL,"
            "PRICE NUMERIC NOT NULL);";

            
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::init_database(connection *C){
    cout << "Initializing database" << endl;
    drop_table(C);
    create_table(C);
    cout << "finish init database" << endl;
}

void Database::insert_account(connection *C, string account_id, string balance){
    cout << "Inserting account" << endl;
    string sql = "INSERT INTO ACCOUNT (ACCOUNT_ID, BALANCE) VALUES ('" + account_id + "', " + balance + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::insert_stock(connection *C, string stock_id, string account_id, double num){
    cout << "Inserting stock" << endl;
    string sql = "INSERT INTO SHARE (SHAREID, ACCOUNT_ID, NUM) VALUES ('" + stock_id + "', '" + account_id + "', " + to_string(num) + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::insert_sell_order(connection *C, string stock_id, string account_id, double num, double price){
    cout << "Inserting sell order" << endl;
    string sql = "INSERT INTO SELL_ORDER (SHARE_ID, ACCOUNT_ID, NUM, PRICE) VALUES ('" + stock_id + "', '" + account_id + "', " + to_string(num) + ", " + to_string(price) + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::insert_buy_order(connection *C, string stock_id, string account_id, double num, double price){
    cout << "Inserting buy order" << endl;
    string sql = "INSERT INTO BUY_ORDER (SHARE_ID, ACCOUNT_ID, NUM, PRICE) VALUES ('" + stock_id + "', '" + account_id + "', " + to_string(num) + ", " + to_string(price) + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::insert_transaction(connection *C, string transaction_id, string account_id, string stock_id, double num, double price, bool isSell){
    cout << "Inserting transaction" << endl;
    string sql = "INSERT INTO TRANSACTION (TRANSACTION_ID, ACCOUNT_ID, SHARE_ID, NUM, PRICE) VALUES ('" + transaction_id + "', '" + account_id + "', '" + stock_id + "', " + to_string(num) + ", " + to_string(price) + ");";
    work W(*C);
    W.exec(sql);
    W.commit();
}


void Database::update_account(connection *C, string account_id, string balance){
    cout << "Updating account" << endl;
    string sql = "UPDATE ACCOUNT SET BALANCE = " + balance + " WHERE ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_stock(connection *C, string stock_id, string account_id, double num){
    cout << "Updating stock" << endl;
    string sql = "UPDATE SHARE SET NUM = " + to_string(num) + " WHERE SHAREID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_sell_order(connection *C, string stock_id, string account_id, double num, double price){
    cout << "Updating sell order" << endl;
    string sql = "UPDATE SELL_ORDER SET NUM = " + to_string(num) + ", PRICE = " + to_string(price) + " WHERE SHARE_ID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_buy_order(connection *C, string stock_id, string account_id, double num, double price){
    cout << "Updating buy order" << endl;
    string sql = "UPDATE BUY_ORDER SET NUM = " + to_string(num) + ", PRICE = " + to_string(price) + " WHERE SHARE_ID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::update_transaction(connection *C, string transaction_id, string account_id, string stock_id, double num, double price, bool isSell){
    cout << "Updating transaction" << endl;
    string sql = "UPDATE TRANSACTION SET NUM = " + to_string(num) + ", PRICE = " + to_string(price) + " WHERE TRANSACTION_ID = '" + transaction_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::delete_account(connection *C, string account_id){
    cout << "Deleting account" << endl;
    string sql = "DELETE FROM ACCOUNT WHERE ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::delete_stock(connection *C, string stock_id){
    cout << "Deleting stock" << endl;
    string sql = "DELETE FROM SHARE WHERE SHAREID = '" + stock_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::delete_sell_order(connection *C, string stock_id, string account_id){
    cout << "Deleting sell order" << endl;
    string sql = "DELETE FROM SELL_ORDER WHERE SHARE_ID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::delete_buy_order(connection *C, string stock_id, string account_id){
    cout << "Deleting buy order" << endl;
    string sql = "DELETE FROM BUY_ORDER WHERE SHARE_ID = '" + stock_id + "' AND ACCOUNT_ID = '" + account_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}

void Database::delete_transaction(connection *C, string transaction_id){
    cout << "Deleting transaction" << endl;
    string sql = "DELETE FROM TRANSACTION WHERE TRANSACTION_ID = '" + transaction_id + "';";
    work W(*C);
    W.exec(sql);
    W.commit();
}




