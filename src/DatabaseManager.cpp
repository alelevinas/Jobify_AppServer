//
// Created by ale on 11/09/16.
//

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(std::string usersDbName,
                                 std::string chatsDbName) {
    users = new UsersDB(usersDbName);
    chats = new ChatsDB(chatsDbName);

    users->openDB();
}

DatabaseManager::~DatabaseManager() {
    delete users;
    delete chats;
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
