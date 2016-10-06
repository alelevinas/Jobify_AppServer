//
// Created by ale on 4/10/16.
//

#ifndef JOBIFY_APPSERVER_SESSIONMANAGER_H
#define JOBIFY_APPSERVER_SESSIONMANAGER_H


#include <databases/SessionsDB.h>

class SessionManager {
    SessionsDB* db;

public:
    SessionManager(SessionsDB *db);
    virtual ~SessionManager();

    std::string add_session(std::string &username, std::string &password);
    bool has_expired(std::string token);

private:
    std::string get_timestamp_now();
    bool timestamp_has_expired(std::string &timestamp);
    std::string get_hashed_usr_pass(std::string &username, std::string &password);
};


#endif //JOBIFY_APPSERVER_SESSIONMANAGER_H
