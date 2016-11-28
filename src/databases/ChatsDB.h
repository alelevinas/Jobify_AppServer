//
// Created by ale on 14/09/16.
//

#ifndef JOBIFY_APPSERVER_CHATSDB_H
#define JOBIFY_APPSERVER_CHATSDB_H


#include <string>
#include <leveldb/db.h>
#include "DB.h"

class ChatsDB: public DB{

public:
    ChatsDB(std::string& dbName);

    virtual ~ChatsDB();

    bool add_msg(std::string basic_string, std::string basicString, std::string basic_string1);

    bool updateMessages(std::string, Json::Value);

    bool get_conv(std::string username, std::string username2, Json::Value *conversation);

    bool get_convs(std::string username, Json::Value *conversations);

    bool delete_conv(std::string username, std::string username2);
};


#endif //JOBIFY_APPSERVER_CHATSDB_H
