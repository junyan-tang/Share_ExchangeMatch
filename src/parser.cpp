#include "parser.hpp"
#include "myStruct.h"
#include <vector>

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
        string balance = account.attribute("balance").value();
        creator.createAccount(account_id, balance);
    }

    for(pugi::xml_node symbol : node.children("symbol")) {
        string sym = symbol.attribute("sym").value();
        for (pugi::xml_node acc : symbol.children("account")) {
            string account_id = acc.attribute("id").value();
            int amount = stoi(acc.child_value());
            creator.createStock(sym, account_id, amount);
        }
    }
}

void XMLParser::processTransaction(const pugi::xml_node &node){
    if(node.attribute("id")){
        if (!node.first_child()) {
            cout << "Invalid transaction" << endl;
            return;
        }
        string transaction_id = node.attribute("id").value();
        pugi::xml_node trans_type = node.first_child();
        while (trans_type){
            if (string(trans_type.name()) == "order"){
                string sym = trans_type.attribute("sym").value();
                string amount = trans_type.attribute("amount").value();
                string limit = trans_type.attribute("limit").value();
            }
            else if (string(trans_type.name()) == "cancel") {
                string transaction_id = trans_type.attribute("id").value();
            }
            else if (string(trans_type.name()) == "query") {
                string transaction_id = trans_type.attribute("id").value();
            }
            trans_type = trans_type.next_sibling();
        }
    }
    else{
        cout << "Invalid transaction" << endl;
        return;
    }
}

string XMLParser::responseForCreate(){
    vector<string> results;
    pugi::xml_document doc;
    auto root = doc.append_child("results");

    for(const auto& result : results) {
        // if result is success
        if (true) {
            auto created = root.append_child("created");
        }
        else {
            auto error = root.append_child("error");
            error.append_attribute("id") = "test";
            error.text().set("test");
        }
    }
}

string XMLParser::responseForTransaction(result R){
    result results;
    pugi::xml_document doc;
    auto root = doc.append_child("results");

    for(const auto& result : results) {
        //if result is success
        if (true) {
            
        }
        else {
            auto error = root.append_child("error");
            error.append_attribute("sym") = "test";
            error.append_attribute("amount") = "test";
            error.append_attribute("limit") = "test";
            error.text().set("test");
        }

    }
    
}