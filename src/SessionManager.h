//
// Created by ale on 4/10/16.
//

#ifndef JOBIFY_APPSERVER_SESSIONMANAGER_H
#define JOBIFY_APPSERVER_SESSIONMANAGER_H

#define ONE_HOUR 3600

#include <databases/SessionsDB.h>
#include "DatabaseManager.h"

class SessionManager {
    DatabaseManager* dbManager;
    double token_duration;

    //TODO: limpiar las sesiones expiradas

public:
    SessionManager(DatabaseManager *dbManager, double token_duration);
    virtual ~SessionManager();

    void set_token_duration(double new_duratio);

    std::string add_session(std::string &username, std::string &password);
    std::string get_username(std::string &token);
    bool has_expired(std::string &token);
    bool delete_session(std::string &token);

private:
    std::string get_timestamp_now();
    bool timestamp_has_expired(std::string &timestamp);
    std::string get_hashed_usr_pass(std::string &username, std::string &password, std::string &timestamp);
    int get_month_index(const std::string & monthName);
};


#endif //JOBIFY_APPSERVER_SESSIONMANAGER_H
