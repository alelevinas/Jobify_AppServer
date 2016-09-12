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
        return false;
    }

    std::ostringstream valueStream;
    valueStream << user;

    s = db->Put(leveldb::WriteOptions(), username, valueStream.str());
    return s.ok();
}



