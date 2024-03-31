#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <pqxx/pqxx>
extern pqxx::connection * root_connection;
extern int transaction_id;
#endif
