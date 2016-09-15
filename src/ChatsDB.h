//
// Created by ale on 14/09/16.
//

#ifndef JOBIFY_APPSERVER_CHATSDB_H
#define JOBIFY_APPSERVER_CHATSDB_H


#include <string>
#include <leveldb/db.h>

class ChatsDB {

    std::string name;
//    leveldb::DB* db;

public:
    ChatsDB(std::string dbName);
};


#endif //JOBIFY_APPSERVER_CHATSDB_H
