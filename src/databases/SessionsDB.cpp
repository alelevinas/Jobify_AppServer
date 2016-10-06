//
// Created by ale on 3/10/16.
//


#include <json/json.h>
#include <exceptions/KeyDoesntExistException.h>
#include "SessionsDB.h"

SessionsDB::SessionsDB(const std::string &db_name) : DB(db_name) {}

SessionsDB::~SessionsDB() {}

bool SessionsDB::add_session(const std::string &token, Json::Value session) {
    return this->add(token, session);
}

//TODO: refactorizar en la clase madre DB
Json::Value SessionsDB::get_session(const std::string &token) {
    std::string session;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), token, &session);

    if (s.IsNotFound()) {
        throw KeyDoesntExistException();
    }
    //return Json::Value(""); //levantar excepcion (?)

    // std::cout << user << std::endl;

    Json::Reader reader;
    Json::Value json_session;
    bool parsingSuccessful = reader.parse(session, json_session);
    if (!parsingSuccessful) {
        std::cerr << reader.getFormattedErrorMessages();
        return false; //levantar excepcion??
    }
    return json_session;
}

bool SessionsDB::delete_session(const std::string &token) {
    leveldb::Status s = db->Delete(leveldb::WriteOptions(),token);

    if (s.IsNotFound())
        return false; //levantar excepcion (?)
    return s.ok();
}