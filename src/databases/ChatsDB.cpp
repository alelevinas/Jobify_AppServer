//
// Created by ale on 14/09/16.
//

#include <iostream>
#include <ctime>
#include "ChatsDB.h"

ChatsDB::ChatsDB(std::string& dbName) : DB(dbName) {

}

ChatsDB::~ChatsDB() {

}

bool ChatsDB::add_msg(std::string user_from, std::string user_to, std::string message) {
    //std::cout << "Mensaje recibido ---" << message << "----" << std::endl;

    Json::Value messageValue;
    messageValue["msg"] = message;
    messageValue["auth"] = user_from;
    messageValue["id"] = 0;

    time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);
    std::stringstream stime;
    stime << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' << now->tm_mday << '-' << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec;
    messageValue["time"] = stime.str();

    std::string sKey1 = user_from + '_' + user_to;
    std::string sKey2 = user_to + '_' + user_from;

    //Para borrar la conversacion
    //this->delete_key(sKey1);
    //this->delete_key(sKey2);

    std::string aux;
    leveldb::Status s1 = db->Get(leveldb::ReadOptions(), sKey1, &aux);
    leveldb::Status s2 = db->Get(leveldb::ReadOptions(), sKey2, &aux);

    std::stringstream ss;
    ss << messageValue;
    std::cout << "-- Valor a agregar: " << ss.str() << " en claves: " << sKey1 << ", " << sKey2 <<"----"<< std::endl;

    if (s1.IsNotFound() != s2.IsNotFound()) {
        return false;
    }
    if (s1.IsNotFound()) {
        Json::Value newValue(Json::arrayValue);
        newValue.append(messageValue);
        return (this->add(sKey1, newValue) && this->add(sKey2, newValue));
    } else {
        return (this->updateMessages(sKey1,messageValue) && this->updateMessages(sKey2,messageValue));
    }
}

bool ChatsDB::updateMessages(std::string sKey, Json::Value messageValue) {
    Json::Value msgAux = this->get(sKey);
    msgAux.append(messageValue);
    return this->update(sKey,msgAux);
}