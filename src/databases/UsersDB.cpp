//
// Created by ale on 11/09/16.
//

#include <iostream>
#include <sstream>
#include <log/easylogging++.h>
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

    ss << "{ \"users\": [";

    std::string separator = "";

    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        ss << separator << it->value().ToString();
        separator = ",";
    }

    ss << "]}";

    if (!it->status().ok()) {
        delete it;
        return "";// Check for any errors found during the scan
    }
    delete it;
    return ss.str();
}

// si el usernameTo ya estaba recomendado por usernameFrom no hace nada y devuelve true
bool UsersDB::recommend_user(const string &usernameFrom, const string &usernameTo) {
    Json::Value destUser = this->get(usernameTo);
    //std::cerr << destUser;
    Json::Value recommended_by = destUser["recommended_by"];//.append(usernameFrom); //listo???
    for (const Json::Value& username : recommended_by) {
        if (username.asString() == usernameFrom) {
            LOG(DEBUG) << " user:" << usernameTo << " ya estaba recommendado por " << usernameFrom;
            return true;
        }
    }

    destUser["recommended_by"].append(usernameFrom);
    LOG(DEBUG) << " user:" << usernameTo << ". Appendeeo a " << recommended_by << " --> " << usernameFrom
               << " y queda " << destUser["recommended_by"] << std::endl;

    return this->edit_user(usernameTo,destUser);
}

bool UsersDB::deRecommend_user(const string &usernameFrom, const string &usernameTo) {
    Json::Value destUser = this->get(usernameTo);
    Json::Value recommended_by = destUser["recommended_by"];//.append(usernameFrom); //listo???

    std::stringstream ss;
    ss << " user:" << usernameTo
       << ". Le quito a " << recommended_by  << " --> " << usernameFrom << " y queda ";

    Json::Value aux;
    for (int i = 0; i < recommended_by.size(); i++) {
        if (recommended_by[i].asString() == usernameFrom) {
            destUser["recommended_by"].removeIndex(i, &aux);
            LOG(DEBUG) << ss.str() << destUser["recommended_by"];
            return this->edit_user(usernameTo,destUser);
        }
    }
    LOG(DEBUG) << ss.str() << destUser["recommended_by"] << " NO ESTABA RECOMENDADO, NO LO SACA";
    return false;
}