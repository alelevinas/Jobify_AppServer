//
// Created by ale on 9/10/16.
//

#ifndef JOBIFY_APPSERVER_ACCOUNTSDB_H
#define JOBIFY_APPSERVER_ACCOUNTSDB_H


#include "DB.h"

class AccountsDB: public DB {
public:
    virtual ~AccountsDB();

public:
    AccountsDB(const std::string &db_name);

    bool add_account(std::string username, std::string password);
    bool is_correct(std::string username, std::string password);

private:
    std::string md5(std::string password);
};


#endif //JOBIFY_APPSERVER_ACCOUNTSDB_H
