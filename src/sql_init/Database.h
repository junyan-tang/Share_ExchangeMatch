//
// Created by Bob G on 2024/3/28.
//

#ifndef SHARE_EXCHANGEMATCH_DATABASE_H
#define SHARE_EXCHANGEMATCH_DATABASE_H \






class Database {
private:
    connection *c;

public:
    Database();

    init(){
        c = new connection("dbname=mysql user=username password=passw0rd host=stock_db port1234");
        if(C->is_open()) {
            cout << "Opened database successfully: " << C->dbname() << endl;
        } else{
            cout << "Can't open database" << endl;
            return;
        }
    }

    create_databse() {
        work W(*C);

        String sql ="";
        sql +=  "CREATE TABLE ACCOUNT("
                "ACCOUNT_ID STRING PRIMARY KEY NOT NULL,"
                "BALANCE STRING NOT NULL);";

        sql +=  "CREATE TABLE STOCK("
                "STOCK_ID STRING PRIMARY KEY NOT NULL,"
                "ACCOUNT_ID STRING REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL,"
                "NUM INT NOT NULL);";

        W.exec(sql);
        W.commit();
        cout << "Table created successfully" << endl;
    }






    }


};




#endif //SHARE_EXCHANGEMATCH_DATABASE_H
