//
// Created by ale on 11/09/16.
//

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(std::string accountsDbName, std::string usersDbName, std::string sessionsDBname,
                                 std::string chatsDbName) {
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
    return users->add_user(username, user);
}

bool DatabaseManager::delete_user(const string &username) {
    return users->delete_user(username);
}

bool DatabaseManager::edit_user(const string &username, Json::Value userEdited) {
    return users->edit_user(username, userEdited);
}

bool DatabaseManager::get_users(Json::Value& result) {
    return users->get_users(result);
}

bool DatabaseManager::recommend_user(const string &usernameFrom, const string &usernameTo) {
    return users->recommend_user(usernameFrom, usernameTo);
}

bool DatabaseManager::deRecommend_user(const string &usernameFrom, const string &usernameTo) {
    return users->deRecommend_user(usernameFrom, usernameTo);
}

bool DatabaseManager::addContact(const string &usernameFrom, const string &usernameTo) {
    return users->addContact(usernameFrom, usernameTo);
}

bool DatabaseManager::removeContact(const string &usernameFrom, const string &usernameTo) {
    return users->removeContact(usernameFrom, usernameTo);
}

bool DatabaseManager::add_session(const std::string &token, Json::Value session) {
    return sessions->add_session(token, session);
}

Json::Value DatabaseManager::get_session(const std::string &token) {
    return sessions->get_session(token);
}

bool DatabaseManager::delete_session(const std::string &token) {
    return sessions->delete_session(token);
}

bool DatabaseManager::add_account(string username, string password) {
    return accounts->add_account(username, password);
}

bool DatabaseManager::is_correct(std::string username, std::string password) {
    return accounts->is_correct(username, password);
}

Json::Value DatabaseManager::get_users_by(string sort_by, string nFilter, string job, string skill, Json::Value &result) {
    return users->get_users_by(sort_by, nFilter, job, skill, result);
}

bool DatabaseManager::add_msg(string user_from, string user_to, string message) {
    return chats->add_msg(user_from, user_to, message);
}

bool DatabaseManager::get_conv(string username, string username2, Json::Value *conversation) {
    return chats->get_conv(username, username2, conversation);
}

bool DatabaseManager::get_convs(string username, Json::Value *conversations) {
    return chats->get_convs(username, conversations);
}

