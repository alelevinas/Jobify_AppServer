//
// Created by ale on 11/09/16.
//

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(std::string usersDbName, std::string sessionsDBname,
                                 std::string chatsDbName, std::string accountsDbName) {
    accounts = new AccountsDB(accountsDbName);
    users = new UsersDB(usersDbName);
    sessions = new SessionsDB(sessionsDBname);
    chats = new ChatsDB(chatsDbName);
}

DatabaseManager::~DatabaseManager() {
    delete accounts;
    delete users;
    delete sessions;
    delete chats;
}

bool DatabaseManager::openDBs() {
    return accounts->openDB() and users->openDB() and sessions->openDB() and chats->openDB();
}

Json::Value DatabaseManager::get_user(const string &username) {
    return users->get_user(username);
}

bool DatabaseManager::add_user(const string &username, Json::Value user) {
    return users->add_user(username,user);
}

bool DatabaseManager::delete_user(const string &username) {
    return users->delete_user(username);
}

bool DatabaseManager::edit_user(const string &username, Json::Value userEdited) {
    return users->edit_user(username,userEdited);
}

std::string DatabaseManager::get_users() {
    return users->get_users();
}

bool DatabaseManager::add_session(const std::string &token, Json::Value session) {
    return sessions->add_session(token,session);
}

Json::Value DatabaseManager::get_session(const std::string &token) {
    return sessions->get_session(token);
}

bool DatabaseManager::delete_session(const std::string &token) {
    return sessions->delete_session(token);
}

bool DatabaseManager::add_account(string username, string password) {
    return accounts->add_account(username,password);
}

bool DatabaseManager::is_correct(std::string username, std::string password) {
    return accounts->is_correct(username,password);
}