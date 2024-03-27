#include "parser.hpp"

void XMLParser::parseRequest(string xml){
    pugi::xml_document doc;
    if(!doc.load_string(xml.c_str())){
        cout << "Invalid XML" << endl;
        return;
    }
    if (doc.child("create")){
        processCreate(doc.child("create"));
    } else if (doc.child("transaction")){
        processTransaction(doc.child("transaction"));
    } else {
        cout << "Invalid request" << endl;
    }
}

void XMLParser::processCreate(const pugi::xml_node &node){
    for(pugi::xml_node account : node.children("account")){
        string account_id = account.attribute("id").value();
        int balance = stoi(account.attribute("balance").value());
    }

    for(pugi::xml_node symbol : node.children("symbol")) {
        string sym = symbol.attribute("sym").value();
        for (pugi::xml_node acc : symbol.children("account")) {
            string account_id = acc.attribute("id").value();
            int amount = stoi(acc.child_value());
        }
    }
}

void XMLParser::processTransaction(const pugi::xml_node &node){
    if(node.attribute("id")){
        string transaction_id = node.attribute("id").value();
        for(pugi::xml_node order : node.children("order")) {
            string sym = order.attribute("sym").value();
            int amount = stoi(order.attribute("amount").value());
            int limit = stoi(order.attribute("limit").value());
        }

        for(pugi::xml_node cancel : node.children("cancel")) {
            string transaction_id = cancel.attribute("id").value();
        }

        for(pugi::xml_node query : node.children("query")) {
            string transaction_id = query.attribute("id").value();
        }
    }
    else{
        cout << "Invalid transaction" << endl;
        return;
    }
}

string XMLParser::generateResponse(){

}