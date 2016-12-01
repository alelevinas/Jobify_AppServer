//
// Created by ale on 11/09/16.
//

#ifndef JOBIFY_APPSERVER_DATABASEMANAGER_H
#define JOBIFY_APPSERVER_DATABASEMANAGER_H


#include <databases/SessionsDB.h>
#include <databases/AccountsDB.h>
#include <databases/ImagesDB.h>
#include "databases/UsersDB.h"
#include "databases/ChatsDB.h"

class DatabaseManager {

//    static DatabaseManager* instance;

    AccountsDB* accounts;
    UsersDB* users;
    SessionsDB* sessions;
    ChatsDB* chats;
    ImagesDB* images;

public:
    DatabaseManager(std::string usersDbName, std::string sessionsDbName, std::string chatsDbName,
                    std::string accountsDbName, std::string imagesDbName);

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

    bool add_msg(string basic_string, string basicString, string basic_string1);

    bool get_conv(string username, string username2, Json::Value *conversation);

    bool get_convs(string username, Json::Value *conversations);

    bool delete_conv(string username, string username2);

    bool delete_msg(string username1, string username2, string idMensaje);

    bool
    get_users_by(string sort_by, int nFilter, string job, string skill, Json::Value& result, int i, string basic_string);

    Json::Value get_image(string username);

    bool add_image(const std::string &username, Json::Value &image);

    bool delete_image(const std::string &username);

    void filter_pos(Json::Value &result, int nDistance, string coordenates);

    void sort_by_distance(Json::Value &result, string coordenates);
};


#endif // JOBIFY_APPSERVER_DATABASEMANAGER_H
