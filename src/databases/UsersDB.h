//
// Created by ale on 11/09/16.
//

#ifndef JOBIFY_APPSERVER_USERSDB_H
#define JOBIFY_APPSERVER_USERSDB_H

#include <string>

#include <json/json.h>
#include "leveldb/db.h"
#include "DB.h"

using std::string;
/**
 * DB that stores all the user information (except the password and session data). A typical user json can be found at
 * the API documentation. the json fields are:
 * "chats" : [],
 *	"city" : "Rosario",
 *	"contacts" : [],
 *	"dob" : "1993-08-19",
 *	"gender" : "male",
 *	"name" : "Alejandro Pablo Levinas",
 *	"nationality" : "argentino",
 *	"previous_exp" : [one,two,three],
 *  "profile" : "Soy un estudiante de ingenieria en informatica que se propone............blabllbla...........",
 *	"recomendations" : [],
 *	"skills" :
 *	[
 *		"C",
 *		"C++",
 *		"GoogleTest"
 *	],
 *	"email" : "alelevinasATgmail.com",
 *  "username" : "alelevinas@gmail.com"
 * }
 */
class UsersDB: public DB {

private:
    bool addContact(Json::Value &user, const string &usernameToAdd);
    bool removeContact(Json::Value &user, const string &usernameToRemove);

public:
    virtual ~UsersDB();
    UsersDB(const string &db_name);

    /**
     * Adds an user to the db
     * @param username
     * @param user
     * @return true if no db errors
     * @throws KeyAlreadyExistsException
     */
    bool add_user(const string &username, Json::Value user);

    /**
     * Gets a user from the db.
     * @param username of the User to return.
     * @return a copy of the json user.
     * @throws KeyDoesntExistException
     */
    Json::Value get_user(const string &username);

    /**
     * Edits a user on the db
     * @param username
     * @param userEdited the json of the hole user eddited
     * @return true if no db errors
     * @throws KeyDoesntExistException
     */
    bool edit_user(const string &username, Json::Value userEdited);

    /**
     * Deletes an User from the db
     * @param username
     * @return true if no db errors
     */
    bool delete_user(const string &username);

    /**
     * Get all users from database
     * @param result to store the result data
     * @return true if no db errors
     */
    bool get_users(Json::Value& result);

    /**
     * Recommends an user. Adds username to de "recommended_by" field
     * @param usernameFrom the User who recommends
     * @param usernameTo the recommended User
     * @return true if no db errors
     */
    bool recommend_user(const string &usernameFrom, const string &usernameTo);

    /**
     * De recommends an user. Deletes the @param usernameFrom from "recommended_by" field
     * of the user @param usernameTo
     * @param usernameFrom The user who derecommends
     * @param usernameTo the derecommendated user
     * @return true if no db errors
     */
    bool deRecommend_user(const string &usernameFrom, const string &usernameTo);

    /**
     * Adds an user as a contact. Adds username to the "contacts" field
     * @param usernameFrom
     * @param usernameTo
     * @return
     */
    bool addContact(const string &usernameFrom, const string &usernameTo);
    bool removeContact(const string &usernameFrom, const string &usernameTo);

    bool parse_json_array(std::string body, Json::Value& result);

    void filter_job(Json::Value &result, string job);

    void filter_skill(Json::Value &result, string skill);
    void sort_by(Json::Value &result, string sorting);

    void top_k(Json::Value &result, int n);

    void sort_by_distance(Json::Value &users, std::string caller_coordenates);

    bool get_users_by(string sorting, int nFilter, string job, string skill,
                      Json::Value& result, int nDistance, string caller_coordenates);

    void filter_pos(Json::Value &result, int nDistance, string caller_coordenates);

    bool isInRange(string pos1, string pos2, int nDistance);

    void getCoordinates(string coordenates, double *latitud, double * longitud);

    double calcularDistancia(string coord_user, string callerCoordenates);

    bool get_user_contacts(string &username, Json::Value &contacts);
};


#endif //JOBIFY_APPSERVER_USERSDB_H
