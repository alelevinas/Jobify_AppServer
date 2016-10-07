//
// Created by ale on 11/09/16.
//

#include <iostream>
#include <sstream>
#include "UsersDB.h"
#include "exceptions/KeyDoesntExistException.h"

using std::cerr;
using std::endl;

UsersDB::UsersDB(const string &db_name) : DB(db_name) {
}

UsersDB::~UsersDB() {

//    delete db; lo hace la clase madre
}

bool UsersDB::add_user(const string &username, Json::Value user) {
    return this->add(username,user);

    /*std::string value;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), username, &value);

    if (!s.IsNotFound()){
        throw KeyAlreadyExistsException();
        //  return false; //  levantar excepcion (?)
    }

    std::ostringstream valueStream;
    valueStream << user;

    s = db->Put(leveldb::WriteOptions(), username, valueStream.str());
    return s.ok();*/
}

Json::Value UsersDB::get_user(const string &username) {
    std::string user;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), username, &user);

    if (s.IsNotFound()) {
        throw KeyDoesntExistException();
    }
        //return Json::Value(""); //levantar excepcion (?)

    // std::cout << user << std::endl;

    Json::Reader reader;
    Json::Value json_user;
    bool parsingSuccessful = reader.parse( user, json_user);
    if (!parsingSuccessful) {
        cerr << reader.getFormattedErrorMessages();
        return false; //levantar excepcion??
    }
    return json_user;
}

bool UsersDB::edit_user(const string &username, Json::Value userEdited) {
    if (!this->delete_user(username))
        return false;
    return this->add_user(username,userEdited);
    //TODO: chequear que sea el mismo, aunque con el token bastaria
}

bool UsersDB::delete_user(const string &username) {
    leveldb::Status s = db->Delete(leveldb::WriteOptions(),username);

    if (s.IsNotFound())
        return false; //levantar excepcion (?)
    return s.ok();
}

