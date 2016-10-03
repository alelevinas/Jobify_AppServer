//
// Created by ale on 3/10/16.
//

#ifndef JOBIFY_APPSERVER_DB_H
#define JOBIFY_APPSERVER_DB_H


#include <leveldb/db.h>

class DB {

protected:
    leveldb::DB* db;
    std::string db_name;


public:
    virtual ~DB();
    DB(const std::string &db_name);

    bool openDB();

};


#endif //JOBIFY_APPSERVER_DB_H
