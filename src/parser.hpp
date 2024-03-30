#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "pugixml.hpp"
#include "business.hpp"
#include <iostream>

using namespace std;
class XMLParser {
private:
    Creation creator;
    Transaction transactor;
public:
    string parseRequest(string xml);
    vector<ResultC> processCreate(const pugi::xml_node &node);
    void processTransaction(const pugi::xml_node &node);
    string responseForCreate(vector<ResultC> results);
    string responseForOrder();
    string responseForTransaction(result R);
    string generateResponse();
};

#endif