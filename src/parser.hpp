#include "pugixml.hpp"
#include <iostream>

using namespace std;
class XMLParser {
public:
    void parseRequest(string xml);
    void processCreate(const pugi::xml_node &node);
    void processTransaction(const pugi::xml_node &node);
    string responseForCreate();
    string responseForOrder();
    string responseForTransaction();
    string generateResponse();
};