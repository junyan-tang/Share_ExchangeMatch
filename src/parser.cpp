#include "parser.hpp"
#include "myStruct.h"
#include <vector>

string XMLParser::parseRequest(string xml){
    pugi::xml_document doc;
    string response;
    if(!doc.load_string(xml.c_str())){
        cout << "Invalid XML" << endl;
        return;
    }
    if (doc.child("create")){
        vector<ResultC> creation_result =  processCreate(doc.child("create"));
        response = responseForCreate(creation_result);
    } else if (doc.child("transaction")){
        processTransaction(doc.child("transaction"));
    } else {
        cout << "Invalid request" << endl;
    }
    return response;
}

vector<ResultC> XMLParser::processCreate(const pugi::xml_node &node){
    vector<ResultC> results;
    if(!node.first_child()){
        return results;
    }
    pugi::xml_node create_type = node.first_child();
    while (create_type){
        if (string(create_type.name()) == "account"){
            string account_id = create_type.attribute("id").value();
            string balance = create_type.attribute("balance").value();
            ResultC current_result = creator.createAccount(account_id, balance);
            results.push_back(current_result);
        }
        else if (string(create_type.name()) == "symbol") {
            string sym = create_type.attribute("sym").value();
            for (pugi::xml_node acc : create_type.children("account")) {
                string account_id = acc.attribute("id").value();
                double amount = stod(acc.child_value());
                ResultC current_result = creator.createStock(sym, account_id, amount);
                results.push_back(current_result);
            }
        }
        create_type = create_type.next_sibling();
    }
    return results;
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

string XMLParser::responseForCreate(vector<ResultC> results){
    pugi::xml_document doc;
    auto root = doc.append_child("results");

    for(const auto& result : results) {
        if (result.status == "success") {
            auto created = root.append_child("created");
            if (result.sym != "") {
                created.append_attribute("sym") = result.sym.c_str();
            }
            created.append_attribute("id") = result.account_id.c_str();
        }
        else {
            auto error = root.append_child("error");
            if (result.sym != "") {
                error.append_attribute("sym") = result.sym.c_str();
            }
            error.append_attribute("id") = result.account_id.c_str();
            error.text().set(result.message.c_str());
        }
    }
    stringstream ss;
    doc.save(ss);
    return ss.str();
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