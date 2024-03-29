#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "pugixml.hpp"
#include "helper.hpp"
#include <iostream>

using namespace std;
class XMLParser {
private:
    Creation creator;
public:
    void parseRequest(string xml);
    void processCreate(const pugi::xml_node &node);
    void processTransaction(const pugi::xml_node &node);
    string responseForCreate();
    string responseForOrder();
    string responseForTransaction(result R);
    string generateResponse();
};

#endif