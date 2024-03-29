//
// Created by Bob G on 2024/3/28.
//

#ifndef SHARE_EXCHANGEMATCH_MYSTRUCT_H
#define SHARE_EXCHANGEMATCH_MYSTRUCT_H

struct Account {
    string account_id;
    string balance;
};

struct Stock {
    string sym;
    string account_id;
    int amount;
};

struct Order {
    string stock_id;
    string account_id;
    int num;
    double price;
    bool isSell;
};


#endif //SHARE_EXCHANGEMATCH_MYSTRUCT_H
