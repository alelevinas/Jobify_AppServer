//
// Created by ale on 11/09/16.
//

#ifndef JOBIFY_APPSERVER_USERSDB_H
#define JOBIFY_APPSERVER_USERSDB_H

#include <string>

#include <json/json.h>
#include "leveldb/db.h"

using std::string;

class UsersDB {

    leveldb::DB* db;
    string db_name;


public:
    virtual ~UsersDB();
    UsersDB(const string &db_name);

    bool openDB();
    bool add_user(const string &username, Json::Value user);
    Json::Value get_user(const string &username);



};


#endif //JOBIFY_APPSERVER_USERSDB_H
