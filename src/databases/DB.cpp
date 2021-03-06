//
// Created by ale on 3/10/16.
//

#include <iostream>
#include <exceptions/KeyAlreadyExistsException.h>
#include <exceptions/KeyDoesntExistException.h>
#include <log/easylogging++.h>
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
        LOG(INFO) << "Unable to open/create database " << db_name << std::endl;
        std::cerr << status.ToString() << std::endl;
        return false;
    }
    return true;
}

bool DB::deleteDB() {
    std::vector<std::string> keys(10);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        keys.push_back(it->key().ToString());
    }
    if(!it->status().ok())
        return false;  // Check for any errors found during the scan
    delete it;

    for (std::string key : keys) {
        leveldb::Status s = db->Delete(leveldb::WriteOptions(), key);
        if (!s.ok())
            return false;
    }
    return true;
}

bool DB::add(const std::string &key, Json::Value value) {
    std::string aux;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &aux);

    if (!s.IsNotFound()){
        throw KeyAlreadyExistsException();
    }

    std::ostringstream valueStream;
    valueStream << value;

//    LOG(DEBUG) << "Adding " << value;

    s = db->Put(leveldb::WriteOptions(), key, valueStream.str());
    return s.ok();
}

bool DB::update(const std::string &key, Json::Value value) {
    std::ostringstream valueStream;
    valueStream << value;

    leveldb::Status s = db->Put(leveldb::WriteOptions(), key, valueStream.str());
    return s.ok();
}

Json::Value DB::get(const std::string &key) {
    std::string value;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &value);

    if (s.IsNotFound()) {
        throw KeyDoesntExistException();
    }

    Json::Reader reader;
    Json::Value json_value;
    bool parsingSuccessful = reader.parse( value, json_value);
    if (!parsingSuccessful) {
        std::cerr << reader.getFormattedErrorMessages();
        return false;
    }
    return json_value;
}

//It is not an error if "key" did not exist in the database. De la doc de leveldb
bool DB::delete_key(const std::string &key) {
    leveldb::Status s = db->Delete(leveldb::WriteOptions(),key);
    return s.ok();
}