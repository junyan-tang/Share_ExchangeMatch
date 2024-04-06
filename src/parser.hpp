#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "pugixml.hpp"
#include "business.hpp"
#include <iostream>
#include "parser.hpp"

using namespace std;
class XMLParser {
public:
    Creation creator;
    Transact transactor;
    connection * C = root_connection;

    string parseRequest(string xml);
    vector<ResultC> processCreate(const pugi::xml_node &node);
    vector<ResultT> processTransaction(const pugi::xml_node &node);
    string responseForCreate(vector<ResultC> results);
    string responseForTransaction(vector<ResultT> results);
};

#endif