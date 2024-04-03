#include "parser.hpp"
#include "myStruct.h"
#include <vector>

// int transaction_id = 0;
string XMLParser::parseRequest(string xml){
    pugi::xml_document doc;
    string response;
    //cout << "Parsing request" << endl;
    if(!doc.load_string(xml.c_str())){
        return "invalid xml";
    }
    if (doc.child("create")){
        vector<ResultC> creation_result =  processCreate(doc.child("create"));
        response = responseForCreate(creation_result);
    } 
    else if (doc.child("transactions")){
        //cout << "Processing transaction" << endl;
        vector<ResultT> transaction_result = processTransaction(doc.child("transactions"));
        //cout << "Generating response" << endl;
        response = responseForTransaction(transaction_result);
        //cout<< "Response generated" << endl;
    } else {
        return "invalid request";
    }
    return response;
}

// vector<ResultC> XMLParser::generateCreate(string xml){
//     pugi::xml_document doc;
//     vector<ResultC> results;
//     if(!doc.load_string(xml.c_str())){
//         cout << "Invalid XML" << endl;
//         return results;
//     }
//     if (doc.child("create")){
//         results = processCreate(doc.child("create"));
//     } else {
//         cout << "Invalid request" << endl;
//     }
//     return results;
// }

// vector<ResultT> XMLParser::generateTransaction(string xml){
//     pugi::xml_document doc;
//     vector<ResultT> results;
//     if(!doc.load_string(xml.c_str())){
//         cout << "Invalid XML" << endl;
//         return results;
//     }
//     if (doc.child("transaction")){
//         results = processTransaction(doc.child("transaction"));
//     } else {
//         cout << "Invalid request" << endl;
//     }
//     return results;
// }

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
            if (!create_type.first_child()) {
                ResultC current_result = {"", sym, "error", "No account included"};
                results.push_back(current_result);
            }
            for (pugi::xml_node acc : create_type.children("account")) {
                string account_id = acc.attribute("id").value();
                double amount = stod(acc.child_value());
                if(amount < 0){
                    ResultC current_result = {"", sym, "error", "Shares cannot be negative"};
                    results.push_back(current_result);
                }
                else{
                    ResultC current_result = creator.createStock(sym, account_id, amount);
                    results.push_back(current_result);
                }   
            }
        }
        create_type = create_type.next_sibling();
    }
    return results;
}

vector<ResultT> XMLParser::processTransaction(const pugi::xml_node &node){
    vector<ResultT> results;
    transaction_id++;
    if(node.attribute("id")){
        string account_id = node.attribute("id").value();
        if(!transactor.checkAccount(account_id)){
            pugi::xml_node curr_node = node.first_child();
            while(curr_node){
                ResultT current_result = {"", "", stoi(account_id), "", "error", "This account is invalid", {}};
                results.push_back(current_result);
                curr_node = curr_node.next_sibling();
            }
            return results;
        }
        if (!node.first_child()) {
            ResultT current_result = {"", "", transaction_id, "", "error", "No transaction included", {}};
            results.push_back(current_result);
            return results;
        }
        pugi::xml_node trans_type = node.first_child();
        while (trans_type){
            cout << "Processing transaction test test" << endl;
            if (string(trans_type.name()) == "order"){
                cout << "Processing order" << endl;
                string sym = trans_type.attribute("sym").value();
                string amount = trans_type.attribute("amount").value();
                string limit = trans_type.attribute("limit").value();
                ResultT current_result = transactor.openOrder(account_id, sym, amount, limit, transaction_id);
                results.push_back(current_result);
            }
            else if (string(trans_type.name()) == "cancel") {
                cout << "Processing cancel" << endl;
                string transaction_id = trans_type.attribute("id").value();
                ResultT current_result = transactor.cancelOrder(stoi(transaction_id));
                results.push_back(current_result);
            }
            else if (string(trans_type.name()) == "query") {
                cout << "Processing query" << endl;
                string transaction_id = trans_type.attribute("id").value();
                ResultT current_result = transactor.queryOrder(stoi(transaction_id));
                results.push_back(current_result);
            }
            trans_type = trans_type.next_sibling();
        }
    }
    else{
        ResultT current_result = {"", "", transaction_id, "", "error", "No account id included", {}};
        results.push_back(current_result);
    }
    return results;
}

string XMLParser::responseForCreate(vector<ResultC> results){
    pugi::xml_document doc;

    auto declaration = doc.prepend_child(pugi::node_declaration);
    declaration.append_attribute("version") = "1.0";
    declaration.append_attribute("encoding") = "UTF-8";

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

string XMLParser::responseForTransaction(vector<ResultT> results){
    pugi::xml_document doc;

    auto declaration = doc.prepend_child(pugi::node_declaration);
    declaration.append_attribute("version") = "1.0";
    declaration.append_attribute("encoding") = "UTF-8";

    auto root = doc.append_child("results");
    cout << results.size() << endl;

    for(const ResultT& result : results) {
        cout << result.status << endl;
        cout << result.message << endl;
        if (result.status == "success") {
            if (result.transaction_type == "order") {
                cout << "Order success" << endl;
                auto opened = root.append_child("opened");
                opened.append_attribute("sym") = result.transaction[0].stock_id.c_str();
                opened.append_attribute("amount") = result.transaction[0].num;
                opened.append_attribute("limit") = result.transaction[0].price;
                opened.append_attribute("id") = to_string(result.trans_id).c_str();

            }
            else{
                pugi::xml_node operation;
                if(result.transaction_type == "cancel"){
                    cout << "Cancel success" << endl;
                    operation = root.append_child("canceled");
                }
                else if(result.transaction_type == "query"){
                    cout << "Query success" << endl;
                    operation = root.append_child("status");
                }
                for (const auto& transaction : result.transaction) {
                    if (transaction.status == "open") {
                        operation.append_child("open")
                                .append_attribute("shares") = transaction.num;
                    }
                    else if(transaction.status == "canceled"){
                        auto cancel = operation.append_child("canceled");
                        cancel.append_attribute("shares") = transaction.num;
                        cancel.append_attribute("time") = transaction.order_time.c_str();
                    }
                    else if(transaction.status == "executed") {
                        auto executed = operation.append_child("executed");
                        executed.append_attribute("shares") = transaction.num;     
                        executed.append_attribute("price") = transaction.price;
                        executed.append_attribute("time") = transaction.order_time.c_str();
                    }
                }
            }
        }

        else {
            auto error = root.append_child("error");
            if (result.transaction_type == "order") {
                error.append_attribute("sym") = result.transaction[0].stock_id.c_str();
                error.append_attribute("amount") = result.transaction[0].num;
                error.append_attribute("limit") = result.transaction[0].price;
                error.text().set(result.message.c_str());
            }
            else{
                error.append_attribute("id") =  to_string(result.trans_id).c_str();
                error.text().set(result.message.c_str());
            }
        }
    }
    stringstream ss;
    doc.save(ss);
    return ss.str();
}

bool XMLParser::isCreate(string xml){
    pugi::xml_document doc;
    if(!doc.load_string(xml.c_str())){
        cout << "Invalid XML" << endl;
        return false;
    }
    if (doc.child("create")){
        return true;
    }
    return false;
}

bool XMLParser::isTransaction(string xml){
    pugi::xml_document doc;
    if(!doc.load_string(xml.c_str())){
        cout << "Invalid XML" << endl;
        return false;
    }
    if (doc.child("transaction")){
        return true;
    }
    return false;
}