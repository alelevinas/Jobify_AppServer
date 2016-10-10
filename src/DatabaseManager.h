//
// Created by ale on 11/09/16.
//

#ifndef JOBIFY_APPSERVER_DATABASEMANAGER_H
#define JOBIFY_APPSERVER_DATABASEMANAGER_H


#include <databases/SessionsDB.h>
#include <databases/AccountsDB.h>
#include "databases/UsersDB.h"
#include "databases/ChatsDB.h"

class DatabaseManager {

//    static DatabaseManager* instance;

    AccountsDB* accounts;
    UsersDB* users;
    SessionsDB* sessions;
    ChatsDB* chats;

public:
    DatabaseManager(std::string usersDbName, std::string sessionsDbName, std::string chatsDbName,
                    std::string accountsDbName);

    virtual ~DatabaseManager();

    bool openDBs();

    //UsersDB
    bool add_user(const string &username, Json::Value user);
    Json::Value get_user(const string &username);
    bool edit_user(const string &username, Json::Value userEdited);
    bool delete_user(const string &username);
    std::string get_users();

    //SessionsDB
    bool add_session(const std::string &token, Json::Value session);
    Json::Value get_session(const std::string &token);
    bool delete_session(const std::string &token);

    //accounts
    bool add_account(string username, string password);
    bool is_correct(std::string username, std::string password);
};


#endif // JOBIFY_APPSERVER_DATABASEMANAGER_H
