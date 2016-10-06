//
// Created by ale on 3/10/16.
//

#include <iostream>
#include <exceptions/KeyAlreadyExistsException.h>
#include "DB.h"

DB::DB(const std::string &db_name) : db_name(db_name) {
}

DB::~DB() {
    delete db;
}

bool DB::openDB() {
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, db_name, &db);

    if (! status.ok()) {
        std::cerr << "Unable to open/create database " << db_name << std::endl;
        std::cerr << status.ToString() << std::endl;
        return false;
    }
    return true;
}

bool DB::add(const std::string &key, Json::Value value) {
    std::string aux;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &aux);

    if (!s.IsNotFound()){
        throw KeyAlreadyExistsException();
        //  return false; //  levantar excepcion (?)
    }

    std::ostringstream valueStream;
    valueStream << value;

    s = db->Put(leveldb::WriteOptions(), key, valueStream.str());
    return s.ok();
}