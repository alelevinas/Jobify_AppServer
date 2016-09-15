//
// Created by ale on 11/09/16.
//

#include <iostream>
#include <sstream>
#include "UsersDB.h"
using std::cerr;
using std::endl;

UsersDB::UsersDB(const string &db_name) : db_name(db_name) {
}

UsersDB::~UsersDB() {
    delete db;
}

bool UsersDB::openDB() {
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, db_name, &db);

    if (! status.ok()) {
        cerr << "Unable to open/create database " << db_name << endl;
        cerr << status.ToString() << endl;
        return false;
    }
    return true;
}

bool UsersDB::add_user(const string &username, Json::Value user) {
    std::string value;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), username, &value);

    if (!s.IsNotFound()){
        return false; //levantar excepcion (?)
    }

    std::ostringstream valueStream;
    valueStream << user;

    s = db->Put(leveldb::WriteOptions(), username, valueStream.str());
    return s.ok();
}

Json::Value UsersDB::get_user(const string &username) {
    std::string user;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), username, &user);

    if (s.IsNotFound())
        return Json::Value(""); //levantar excepcion (?)

    // std::cout << user << std::endl;

    Json::Reader reader;
    Json::Value json_user;
    bool parsingSuccessful = reader.parse( user, json_user);
    if (!parsingSuccessful) {
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

