//
// Created by ale on 11/09/16.
//

#ifndef JOBIFY_APPSERVER_DATABASEMANAGER_H
#define JOBIFY_APPSERVER_DATABASEMANAGER_H


#include "databases/UsersDB.h"
#include "databases/ChatsDB.h"

class DatabaseManager {

//    static DatabaseManager* instance;

    UsersDB* users;
    ChatsDB* chats;

public:
    DatabaseManager(std::string usersDbName, std::string chatsDbName);

    virtual ~DatabaseManager();

    //UsersDB
    bool add_user(const string &username, Json::Value user);
    Json::Value get_user(const string &username);
    bool edit_user(const string &username, Json::Value userEdited);
    bool delete_user(const string &username);


};


#endif // JOBIFY_APPSERVER_DATABASEMANAGER_H
