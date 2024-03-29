#include "database.hpp"
#include "server.hpp"

int main(){
    // connection * C = new connection("dbname=postgres user=postgres password=passw0rd");  
    // if(C->is_open()){
    //     cout << "Opened database successfully: " << C->dbname() << endl;
    // }else{
    //     cout << "Can't open database" << endl;
    //     return EXIT_FAILURE;
    // }


    // Database db;
    // db.init_database(C);
    cout << "create server" << endl;
    Server server;

    cout << "run server" << endl;
    server.run();

    
}