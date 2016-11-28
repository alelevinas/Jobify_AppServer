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
    bool get_users(Json::Value& result);
    bool recommend_user(const string &usernameFrom, const string &usernameTo);
    bool deRecommend_user(const string &usernameFrom, const string &usernameTo);
    bool addContact(const string &usernameFrom, const string &usernameTo);
    bool removeContact(const string &usernameFrom, const string &usernameTo);
    //SessionsDB
    bool add_session(const std::string &token, Json::Value session);
    Json::Value get_session(const std::string &token);
    bool delete_session(const std::string &token);

    //accounts
    bool add_account(string username, string password);
    bool is_correct(std::string username, std::string password);

    Json::Value get_users_by(string sort_by, string nFilter, string job, string skill, Json::Value &result);

    bool add_msg(string basic_string, string basicString, string basic_string1);

    bool get_conv(string username, string username2, Json::Value *conversation);

    bool get_convs(string username, Json::Value *conversations);

    bool delete_conv(string username, string username2);

    bool delete_msg(string username1, string username2, string idMensaje);
};


#endif // JOBIFY_APPSERVER_DATABASEMANAGER_H
