#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "pugixml.hpp"
#include "business.hpp"
#include <iostream>

using namespace std;
class XMLParser {
private:
    Creation creator;
    Transact transactor;
public:
    string parseRequest(string xml);
    vector<ResultC> processCreate(const pugi::xml_node &node);
    vector<ResultT> processTransaction(const pugi::xml_node &node);
    string responseForCreate(vector<ResultC> results);
    string responseForTransaction(vector<ResultT> results);
    string generateResponse();
};

#endif