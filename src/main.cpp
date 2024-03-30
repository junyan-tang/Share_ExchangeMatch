#include "database.hpp"
#include "server.hpp"
#include "global_var.hpp"
int main(){
    root_connection = new connection("dbname=postgres user=postgres password=passw0rd");
    if(root_connection->is_open()){
        cout << "Opened database successfully: " << root_connection->dbname() << endl;
    }else{
        cout << "Can't open database" << endl;
        return EXIT_FAILURE;
    }


    Database db;

    cout << "init main database" << endl;
    db.init_database();
    cout << "finish init main database" << endl;

    cout << "create server" << endl;
    Server server;


    server.run();

    
}