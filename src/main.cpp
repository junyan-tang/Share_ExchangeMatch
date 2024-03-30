#include "database.hpp"
#include "server.hpp"

int main(){
    connection * C = new connection("dbname=postgres user=postgres password=passw0rd");  
    if(C->is_open()){
        cout << "Opened database successfully: " << C->dbname() << endl;
    }else{
        cout << "Can't open database" << endl;
        return EXIT_FAILURE;
    }


    Database db;
    cout << "init main database" << endl;
    db.init_database(C);
    cout << "finish init main database" << endl;
    // cout << "create server" << endl;
    // Server server;

    // cout << "run server" << endl;
    // server.run();

    
}