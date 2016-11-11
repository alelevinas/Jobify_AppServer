//
// Created by ale on 11/09/16.
//

#include <iostream>
#include <sstream>
#include "UsersDB.h"

using std::cerr;
using std::endl;

UsersDB::UsersDB(const string &db_name) : DB(db_name) {
}

UsersDB::~UsersDB() {

//    delete db; lo hace la clase madre
}

bool UsersDB::add_user(const string &username, Json::Value user) {
    return this->add(username,user);
}

Json::Value UsersDB::get_user(const string &username) {
    return this->get(username);
}

bool UsersDB::edit_user(const string &username, Json::Value userEdited) {
    if (!this->delete_user(username))
        return false;
    return this->add_user(username,userEdited);
}

bool UsersDB::delete_user(const string &username) {
    return this->delete_key(username);
}

string UsersDB::get_users() {
    std::stringstream ss;

    ss << "[";

    std::string separator = "";

    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        ss << separator << it->value().data();//ToString();
        separator = ",";
    }

    ss << "]";

    if (!it->status().ok()) {
        delete it;
        return "";// Check for any errors found during the scan
    }
    delete it;
    return ss.str();
}