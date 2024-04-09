#include "database.hpp"
#include "server.hpp"
#include "global_var.hpp"


int main()
{

    //change to own database when local testing
    root_connection = new connection("dbname=postgres user=postgres password=passw0rd host=db port=5432");

    if (root_connection->is_open())
    {
        cout << "Opened database successfully: " << root_connection->dbname() << endl;
    }
    else
    {
        cout << "Can't open database" << endl;
        return EXIT_FAILURE;
    }

    Database db;

    db.init_database();
    
    Server server; 
    cout << "Server is running ..." << endl;

    server.run();
}
