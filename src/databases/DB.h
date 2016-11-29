//
// Created by ale on 3/10/16.
//

#ifndef JOBIFY_APPSERVER_DB_H
#define JOBIFY_APPSERVER_DB_H


#include <leveldb/db.h>
#include <json/json.h>

class DB {

protected:
    leveldb::DB* db;
    std::string db_name;
    bool add(const std::string &key, Json::Value value);
    Json::Value get(const std::string &key);
    bool delete_key(const std::string &key);


public:
    virtual ~DB();
    DB(const std::string &db_name);

    bool openDB();

    bool update(const std::string &key, Json::Value value);
};


#endif //JOBIFY_APPSERVER_DB_H
