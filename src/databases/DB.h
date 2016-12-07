//
// Created by ale on 3/10/16.
//

#ifndef JOBIFY_APPSERVER_DB_H
#define JOBIFY_APPSERVER_DB_H


#include <leveldb/db.h>
#include <json/json.h>

/**
 * Base class surrounding methods of the leveldb lib throwing exceptions if an error ocurrs.
 * All values are stored as @class Json::Values
 */
class DB {

protected:
    leveldb::DB* db; /// the actual database
    std::string db_name;

    /**
     * Adds a new key-value to the db
     * @param key
     * @param value
     * @return true if the value with key was added
     * @throws KeyAlreadyExistsException
     */
    bool add(const std::string &key, Json::Value value);

    /**
     * Gets the value of @param key
     * @param key The key
     * @return A copy of the value.
     * @throws KeyDoesntExistException
     */
    Json::Value get(const std::string &key);

    /**
     * Deletes the key-value from the database
     * @param key
     * @return true if no db errors
     */
    bool delete_key(const std::string &key);


public:
    virtual ~DB();
    DB(const std::string &db_name);

    bool openDB();

    /**
     * Updates a value from the db.
     * @param key The key
     * @param value the new value to store
     * @return true if @param value was added with key @param key
     */
    bool update(const std::string &key, Json::Value value);
};


#endif //JOBIFY_APPSERVER_DB_H
