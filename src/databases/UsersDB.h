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

class UsersDB: public DB {

//    leveldb::DB* db;
//    string db_name;

private:
    bool addContact(Json::Value &user, const string &usernameToAdd);
    bool removeContact(Json::Value &user, const string &usernameToRemove);

    //static bool sort_json_array(std::pair <int, Json::Value&> a, std::pair <int, Json::Value&> b);
    //bool sort_json_array(Json::Value& a, Json::Value& b, string sorting);


public:
    virtual ~UsersDB();
    UsersDB(const string &db_name);

//    bool openDB();
    bool add_user(const string &username, Json::Value user);
    Json::Value get_user(const string &username);
    bool edit_user(const string &username, Json::Value userEdited);
    bool delete_user(const string &username); //sacar al user de contacs, etc?

    bool get_users(Json::Value& result);

    bool recommend_user(const string &usernameFrom, const string &usernameTo);
    bool deRecommend_user(const string &usernameFrom, const string &usernameTo);
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
};


#endif //JOBIFY_APPSERVER_USERSDB_H
