//
// Created by ale on 4/10/16.
//

#ifndef JOBIFY_APPSERVER_SESSIONMANAGER_H
#define JOBIFY_APPSERVER_SESSIONMANAGER_H

#define ONE_HOUR 3600

#include <databases/SessionsDB.h>
#include "DatabaseManager.h"

/**
 * The Session Manager manages the users accounts. Checks for username matching password
 * and generates tokens so that clients can acces the API.
 */
class SessionManager {
    DatabaseManager* dbManager;  /* the DB*/
    double token_duration;  /*time until the token expires*/

public:
    SessionManager(DatabaseManager *dbManager, double token_duration);
    virtual ~SessionManager();

    /**
     * Sets the new token duration
     * @param new_duration the new duration
     */
    void set_token_duration(double new_duration);

    /**
     * Adds a new session if password matches username.
     * @param username the username
     * @param password the password
     * @return a token to acces the API.
     * @throws InvalidUsernamePasswordException
     * @throws KeyAlreadyExistsException
     */
    std::string add_session(std::string &username, std::string &password);

    /**
     * Gets the token's username
     * @param token The token generated in add_session
     * @return the username
     * @throws TokenInvalidException
     */
    std::string get_username(std::string &token);

    /**
     * Checks if the token has expired
     * @param token The token generated in add_session
     * @return true if the token expired
     * @throws TokenInvalidException
     */
    bool has_expired(std::string &token);

    /**
     * Deletes the session corresponding to the token
     * @param token The token generated in add_session
     * @return true if the session was deleted correctly
     */
    bool delete_session(std::string &token);

private:
    std::string get_timestamp_now();
    bool timestamp_has_expired(std::string &timestamp);
    std::string get_hashed_usr_pass(std::string &username, std::string &password, std::string &timestamp);
    int get_month_index(const std::string & monthName);
};


#endif //JOBIFY_APPSERVER_SESSIONMANAGER_H
