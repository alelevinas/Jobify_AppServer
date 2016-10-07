//
// Created by ale on 11/09/16.
//

#ifndef JOBIFY_APPSERVER_USERSDB_H
#define JOBIFY_APPSERVER_USERSDB_H

#include <string>

#include <json/json.h>
#include "leveldb/db.h"
#include "DB.h"

using std::string;

class UsersDB: public DB {

//    leveldb::DB* db;
//    string db_name;


public:
    virtual ~UsersDB();
    UsersDB(const string &db_name);

//    bool openDB();
    bool add_user(const string &username, Json::Value user);
    Json::Value get_user(const string &username);
    bool edit_user(const string &username, Json::Value userEdited);
    bool delete_user(const string &username);
};


#endif //JOBIFY_APPSERVER_USERSDB_H
