//
// Created by ale on 9/10/16.
//

#ifndef JOBIFY_APPSERVER_ACCOUNTSDB_H
#define JOBIFY_APPSERVER_ACCOUNTSDB_H


#include "DB.h"

/**
 * DB that saves the username and password (encrypted) data from users.
 */
class AccountsDB: public DB {
public:
    virtual ~AccountsDB();

public:
    AccountsDB(const std::string &db_name);

    /**
     * Adds a username-password entry to the DB
     * @param username the users username
     * @param password the users password
     * @return true if the username doesnt exist
     * @throws KeyAlreadyExistsException
     */
    bool add_account(std::string username, std::string password);

    /**
     * Checks if the password corresponds to that username
     * @param username
     * @param password
     * @return True if it is correct
     * @throws KeyDoesntExistException
     */
    bool is_correct(std::string username, std::string password);

private:
    std::string md5(std::string password);
};


#endif //JOBIFY_APPSERVER_ACCOUNTSDB_H
