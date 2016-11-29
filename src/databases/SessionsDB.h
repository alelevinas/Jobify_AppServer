//
// Created by ale on 3/10/16.
//

#ifndef JOBIFY_APPSERVER_SESSIONSDB_H
#define JOBIFY_APPSERVER_SESSIONSDB_H


#include <leveldb/db.h>
#include <json/json.h>
#include "DB.h"


/*For now, this DB will consist of a generated token and its username.
 * Only the active ones will be on the DB
 * {token: username}
 *
 * TODO: Add metadata and automatic logout (invalidate token) in a certain period of time
 * */
class SessionsDB: public DB {
public:
    virtual ~SessionsDB();

    SessionsDB(const std::string &db_name);

    bool add_session(const std::string &token, Json::Value session);
    Json::Value get_session(const std::string &token);
    bool delete_session(const std::string &token);
};


#endif //JOBIFY_APPSERVER_SESSIONSDB_H
