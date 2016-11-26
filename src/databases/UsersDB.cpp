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
    return this->add(username, user);
}

Json::Value UsersDB::get_user(const string &username) {
    return this->get(username);
}

bool UsersDB::edit_user(const string &username, Json::Value userEdited) {
    if (!this->delete_user(username))
        return false;
    return this->add_user(username, userEdited);
}

bool UsersDB::delete_user(const string &username) {
    return this->delete_key(username);
}

bool UsersDB::get_users(Json::Value& result) {
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
    return parse_json_array(ss.str(), result);
}

// si el usernameTo ya estaba recomendado por usernameFrom no hace nada y devuelve true
bool UsersDB::recommend_user(const string &usernameFrom, const string &usernameTo) {
    Json::Value destUser = this->get(usernameTo);
    //std::cerr << destUser;
    Json::Value recommended_by = destUser["recommended_by"];//.append(usernameFrom); //listo???
    for (const Json::Value &username : recommended_by) {
        if (username.asString() == usernameFrom) {
            LOG(DEBUG) << " user:" << usernameTo << " ya estaba recommendado por " << usernameFrom;
            return true;
        }
    }

    destUser["recommended_by"].append(usernameFrom);
    LOG(DEBUG) << " user:" << usernameTo << ". Appendeeo a " << recommended_by << " --> " << usernameFrom
               << " y queda " << destUser["recommended_by"] << std::endl;

    return this->edit_user(usernameTo, destUser);
}

bool UsersDB::deRecommend_user(const string &usernameFrom, const string &usernameTo) {
    Json::Value destUser = this->get(usernameTo);
    Json::Value recommended_by = destUser["recommended_by"];//.append(usernameFrom); //listo???

    std::stringstream ss;
    ss << " user:" << usernameTo
       << ". Le quito a " << recommended_by << " --> " << usernameFrom << " y queda ";

    Json::Value aux;
    for (int i = 0; i < recommended_by.size(); i++) {
        if (recommended_by[i].asString() == usernameFrom) {
            destUser["recommended_by"].removeIndex(i, &aux);
            LOG(DEBUG) << ss.str() << destUser["recommended_by"];
            return this->edit_user(usernameTo, destUser);
        }
    }
    LOG(DEBUG) << ss.str() << destUser["recommended_by"] << " NO ESTABA RECOMENDADO, NO LO SACA";
    return false;
}

bool UsersDB::addContact(Json::Value &user, const string &usernameToAdd) {
    Json::Value contacts = user["contacts"];
    for (const Json::Value &username : contacts) {
        if (username.asString() == usernameToAdd) {
            LOG(DEBUG) << " user:" << usernameToAdd << " ya era contacto de " << user["username"];
            return true;
        }
    }

    user["contacts"].append(usernameToAdd);
    LOG(DEBUG) << " user:" << user["username"] << ". Appendeeo a " << contacts << " --> " << usernameToAdd
               << " y queda " << user["contacts"] << std::endl;

    return this->edit_user(user["username"].asString(), user);
}


// si el usernameTo ya estaba recomendado por usernameFrom no hace nada y devuelve true
bool UsersDB::addContact(const string &usernameFrom, const string &usernameTo) {
    Json::Value origUser = this->get(usernameFrom);
    Json::Value destUser = this->get(usernameTo);

    if (addContact(origUser, usernameTo)) {
        if (addContact(destUser, usernameFrom))
            return true;
        else
            removeContact(origUser, usernameTo);
    }
    return false;
}

bool UsersDB::removeContact(Json::Value &user, const string &usernameToRemove) {
    Json::Value contacts = user["contacts"];//.append(usernameFrom); //listo???

    std::stringstream ss;
    ss << " user:" << user["username"]
       << ". Le quito a " << contacts << " --> " << usernameToRemove << " y queda ";

    Json::Value aux;
    for (int i = 0; i < contacts.size(); i++) {
        if (contacts[i].asString() == usernameToRemove) {
            user["contacts"].removeIndex(i, &aux);
            LOG(DEBUG) << ss.str() << user["contacts"];
            return this->edit_user(user["username"].asString(), user);
        }
    }
    LOG(DEBUG) << ss.str() << user["contacts"] << " NO ERAN CONTACTOS, NO LO SACA";
    return false;
}

bool UsersDB::removeContact(const string &usernameFrom, const string &usernameTo) {
    Json::Value origUser = this->get(usernameFrom);
    Json::Value destUser = this->get(usernameTo);

    if (removeContact(origUser, usernameTo)) {
        if (removeContact(destUser, usernameFrom))
            return true;
        else
            addContact(origUser, usernameTo);
    }
    return false;
}

bool UsersDB::get_users_by(string sort_by, string nFilter, string job, string skill, Json::Value& result) {
    if(!get_users(result))
        return false;


}

bool UsersDB::parse_json_array(std::string body, Json::Value& result) {
    Json::Reader reader;
    bool ok = true;
    bool parsingSuccessful = reader.parse(body, result);
    return parsingSuccessful;
}
