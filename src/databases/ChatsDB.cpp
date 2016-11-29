//
// Created by ale on 14/09/16.
//

#include <iostream>
#include <ctime>
#include <exceptions/KeyDoesntExistException.h>
#include "ChatsDB.h"
#include "UsersDB.h"

ChatsDB::ChatsDB(std::string& dbName) : DB(dbName) {

}

ChatsDB::~ChatsDB() {

}

bool ChatsDB::add_msg(std::string user_from, std::string user_to, std::string message) {
    Json::Value messageValue;
    messageValue["msg"] = message;
    messageValue["auth"] = user_from;

    time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);
    std::stringstream stime;
    stime << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' << now->tm_mday << '-' << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec;
    messageValue["time"] = stime.str();

    std::string sKey1 = user_from + '_' + user_to;
    std::string sKey2 = user_to + '_' + user_from;

    std::string aux;
    leveldb::Status s1 = db->Get(leveldb::ReadOptions(), sKey1, &aux);
    leveldb::Status s2 = db->Get(leveldb::ReadOptions(), sKey2, &aux);

    std::stringstream ss;
    ss << messageValue;
    std::cout << "-- Valor a agregar: " << ss.str() << " en claves: " << sKey1 << ", " << sKey2 <<"----"<< std::endl;

    bool ok = true;
    if (s1.IsNotFound()) {
        messageValue["id"] = 0;
        Json::Value newValue(Json::arrayValue);
        newValue.append(messageValue);
        ok = this->add(sKey1, newValue) && ok;
    } else {
        ok = this->updateMessages(sKey1,messageValue) && ok;
    }
    if (s2.IsNotFound()) {
        messageValue["id"] = 0;
        Json::Value newValue(Json::arrayValue);
        newValue.append(messageValue);
        ok = this->add(sKey2, newValue) && ok;
    } else {
        ok = this->updateMessages(sKey2,messageValue) && ok;
    }
    return ok;
}

bool ChatsDB::updateMessages(std::string sKey, Json::Value messageValue) {
    Json::Value msgAux = this->get(sKey);
    messageValue["id"] = msgAux.size();
    msgAux.append(messageValue);

    return this->update(sKey,msgAux);
}

bool ChatsDB::get_conv(std::string username, std::string username2, Json::Value *conversation) {
    std::string sKey = username + '_' + username2;
    try {
        *conversation = this->get(sKey);
    } catch (KeyDoesntExistException &e) {
        *conversation = "";
    }
    return true;
}

bool ChatsDB::get_convs(std::string username, Json::Value *conversations, UsersDB *usersDB) {
    std::stringstream ss;

    ss << "{ \"chats\": [";

    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());

    std::string separator = "";

    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if (strncmp(it->key().ToString().c_str(),username.c_str(),username.size()) == 0) {
            std::string user2 = it->key().ToString().substr(username.size()+1);
            ss << separator << "{ \"" << user2 << "\":";
            //ss << it->value().ToString();
            ss << usersDB->get_user(user2);
            ss << "}";
            separator = ",";
        }
    }

    ss << "]}";

    if (!it->status().ok()) {
        delete it;
        return false;
    }
    delete it;

    Json::Reader reader;
    return reader.parse(ss.str(), *conversations);
}

bool ChatsDB::delete_conv(std::string username, std::string username2) {
    std::string sKey1 = username + '_' + username2;
    return this->delete_key(sKey1);
}

bool ChatsDB::delete_message(std::string username1, std::string username2, std::string idMensaje) {
    std::string sKey = username1 + '_' + username2;
    Json::Value conversation = this->get(sKey);
    Json::Value newValue(Json::arrayValue);

    bool deleted = false;
    for (const Json::Value& message : conversation) {
        if (message["id"].asString() == idMensaje) {
            deleted = true;
        } else {
            Json::Value newMessage = message;
            newMessage["id"] = newValue.size();
            newValue.append(newMessage);
        }
    }

    this->update(sKey, newValue);
    return deleted;
}
