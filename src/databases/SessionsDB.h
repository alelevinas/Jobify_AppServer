//
// Created by ale on 3/10/16.
//

#ifndef JOBIFY_APPSERVER_SESSIONSDB_H
#define JOBIFY_APPSERVER_SESSIONSDB_H


#include <leveldb/db.h>

class SessionsDB {

    leveldb::DB* db;
    std::string db_name;

};


#endif //JOBIFY_APPSERVER_SESSIONSDB_H
