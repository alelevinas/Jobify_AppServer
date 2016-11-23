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

private:
    bool addContact(Json::Value &user, const string &usernameToAdd);
    bool removeContact(Json::Value &user, const string &usernameToRemove);


public:
    virtual ~UsersDB();
    UsersDB(const string &db_name);

//    bool openDB();
    bool add_user(const string &username, Json::Value user);
    Json::Value get_user(const string &username);
    bool edit_user(const string &username, Json::Value userEdited);
    bool delete_user(const string &username); //sacar al user de contacs, etc?

    string get_users();

    bool recommend_user(const string &usernameFrom, const string &usernameTo);
    bool deRecommend_user(const string &usernameFrom, const string &usernameTo);
    bool addContact(const string &usernameFrom, const string &usernameTo);
    bool removeContact(const string &usernameFrom, const string &usernameTo);

    Json::Value get_popular_users();
};


#endif //JOBIFY_APPSERVER_USERSDB_H
