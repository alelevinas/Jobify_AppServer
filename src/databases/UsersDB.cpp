//
// Created by ale on 11/09/16.
//

#include <iostream>
#include <sstream>
#include <log/easylogging++.h>
#include "UsersDB.h"
#include "HaversineDistance.h"

using std::cerr;
using std::endl;

UsersDB::UsersDB(const string &db_name) : DB(db_name) {
}

UsersDB::~UsersDB() {

//    delete db; lo hace la clase madre
}

bool UsersDB::add_user(const string &username, Json::Value user) {
    return this->add(username, user);
}

Json::Value UsersDB::get_user(const string &username) {
    return this->get(username);
}

bool UsersDB::edit_user(const string &username, Json::Value userEdited) {
    if (!this->delete_user(username))
        return false;
    return this->add_user(username, userEdited);
}

bool UsersDB::delete_user(const string &username) {
    return this->delete_key(username);
}

bool UsersDB::get_users(Json::Value &result) {
    std::stringstream ss;

    ss << "{ \"users\": [";

    std::string separator = "";

    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        ss << separator << it->value().ToString();
        separator = ",";
    }

    ss << "]}";

    if (!it->status().ok()) {
        delete it;
        return "";// Check for any errors found during the scan
    }
    delete it;
    return parse_json_array(ss.str(), result);
}

// si el usernameTo ya estaba recomendado por usernameFrom no hace nada y devuelve true
bool UsersDB::recommend_user(const string &usernameFrom, const string &usernameTo) {
    Json::Value destUser = this->get(usernameTo);
    //std::cerr << destUser;
    Json::Value recommended_by = destUser["recommended_by"];//.append(usernameFrom); //listo???
    for (const Json::Value &username : recommended_by) {
        if (username.asString() == usernameFrom) {
            LOG(DEBUG) << " user:" << usernameTo << " ya estaba recommendado por " << usernameFrom;
            return true;
        }
    }

    destUser["recommended_by"].append(usernameFrom);
    LOG(DEBUG) << " user:" << usernameTo << ". Appendeeo a " << recommended_by << " --> " << usernameFrom
               << " y queda " << destUser["recommended_by"] << std::endl;

    return this->edit_user(usernameTo, destUser);
}

bool UsersDB::deRecommend_user(const string &usernameFrom, const string &usernameTo) {
    Json::Value destUser = this->get(usernameTo);
    Json::Value recommended_by = destUser["recommended_by"];//.append(usernameFrom); //listo???

    std::stringstream ss;
    ss << " user:" << usernameTo
       << ". Le quito a " << recommended_by << " --> " << usernameFrom << " y queda ";

    Json::Value aux;
    for (int i = 0; i < recommended_by.size(); i++) {
        if (recommended_by[i].asString() == usernameFrom) {
            destUser["recommended_by"].removeIndex(i, &aux);
            LOG(DEBUG) << ss.str() << destUser["recommended_by"];
            return this->edit_user(usernameTo, destUser);
        }
    }
    LOG(DEBUG) << ss.str() << destUser["recommended_by"] << " NO ESTABA RECOMENDADO, NO LO SACA";
    return false;
}

bool UsersDB::addContact(Json::Value &user, const string &usernameToAdd) {
    Json::Value contacts = user["contacts"];
    for (const Json::Value &username : contacts) {
        if (username.asString() == usernameToAdd) {
            LOG(DEBUG) << " user:" << usernameToAdd << " ya era contacto de " << user["username"];
            return true;
        }
    }

    user["contacts"].append(usernameToAdd);
    LOG(DEBUG) << " user:" << user["username"] << ". Appendeeo a " << contacts << " --> " << usernameToAdd
               << " y queda " << user["contacts"] << std::endl;

    return this->edit_user(user["username"].asString(), user);
}


// si el usernameTo ya estaba recomendado por usernameFrom no hace nada y devuelve true
bool UsersDB::addContact(const string &usernameFrom, const string &usernameTo) {
    Json::Value origUser = this->get(usernameFrom);
    Json::Value destUser = this->get(usernameTo);

    if (addContact(origUser, usernameTo)) {
        if (addContact(destUser, usernameFrom))
            return true;
        else
            removeContact(origUser, usernameTo);
    }
    return false;
}

bool UsersDB::removeContact(Json::Value &user, const string &usernameToRemove) {
    Json::Value contacts = user["contacts"];//.append(usernameFrom); //listo???

    std::stringstream ss;
    ss << " user:" << user["username"]
       << ". Le quito a " << contacts << " --> " << usernameToRemove << " y queda ";

    Json::Value aux;
    for (int i = 0; i < contacts.size(); i++) {
        if (contacts[i].asString() == usernameToRemove) {
            user["contacts"].removeIndex(i, &aux);
            LOG(DEBUG) << ss.str() << user["contacts"];
            return this->edit_user(user["username"].asString(), user);
        }
    }
    LOG(DEBUG) << ss.str() << user["contacts"] << " NO ERAN CONTACTOS, NO LO SACA";
    return false;
}

bool UsersDB::removeContact(const string &usernameFrom, const string &usernameTo) {
    Json::Value origUser = this->get(usernameFrom);
    Json::Value destUser = this->get(usernameTo);

    if (removeContact(origUser, usernameTo)) {
        if (removeContact(destUser, usernameFrom))
            return true;
        else
            addContact(origUser, usernameTo);
    }
    return false;
}

bool UsersDB::parse_json_array(std::string body, Json::Value &result) {
    Json::Reader reader;
    return reader.parse(body, result);
}

bool UsersDB::get_users_by(string sorting, int nFilter, string job, string skill, Json::Value result, int nDistance, string caller_coordenates) {
    Json::Value users;

    if (!get_users(users))
        return false;

    result.swapPayload(users["users"]);

    filter_pos(result, nDistance, caller_coordenates);
    LOG(DEBUG) << "FILTRADO POR POS \n" << result << std::endl;

    filter_job(result, job);
    LOG(DEBUG) << "FILTRADO EL JOB \n" << result << std::endl;

    filter_skill(result, skill);
    LOG(DEBUG) << "FILTRADO EL SKILL \n" << result << std::endl;

    //primero ordeno por distancia, luego por lo que me haya pedido, si pidio algo
    sort_by_distance(result, caller_coordenates);
    LOG(DEBUG) << "ORDENANDO por distancia\n" << result << std::endl;

    sort_by(result, sorting);
    LOG(DEBUG) << "ORDENANDO por " << sorting << "\n" << result << std::endl;

    top_k(result, nFilter);
    LOG(DEBUG) << "TOP K \n" << result << std::endl;
    return true;
}

void UsersDB::filter_pos(Json::Value &result, int nDistance, std::string caller_coordenates) {
    if(caller_coordenates.empty())
        return;
    Json::Value aux_result(Json::arrayValue);

    for (Json::ValueConstIterator itr = result.begin(); itr != result.end(); itr++) {
        Json::Value user = *itr;

        Json::Value pos(Json::arrayValue);
        pos = user["coordenates"];

        if (isInRange(pos.asString(),caller_coordenates,nDistance)) {
            aux_result.append(user);
        }
    }
    result.swapPayload(aux_result);
}

/*
 * Las posiciones deben tener el formato "xxx:yyy"
 */
bool UsersDB::isInRange(std::string pos1, std::string pos2, int nDistance) {
    double lat1, long1, lat2, long2;
    getCoordinates(pos1, &lat1, &long1);
    getCoordinates(pos2, &lat2, &long2);

    return distanceEarth(lat1, long1, lat2, long2) < nDistance;
}

void UsersDB::getCoordinates(string coordenates, double *latitud, double *longitud) {
    std::stringstream ss(coordenates);
    ss >> *latitud;
    ss.ignore(1);
    ss >> *longitud;
}

void UsersDB::filter_job(Json::Value &result, string job) {
    if (job.empty())
        return;
    Json::Value aux_result(Json::arrayValue);

    for (Json::ValueConstIterator itr = result.begin(); itr != result.end(); itr++) {
        Json::Value user = *itr;

//        std::cerr << "\nBUSCANDO " << job;
//        std::cerr << "\nPREVIOUS EXP --> " << user["previous_exp"];
        Json::Value job_exp(Json::arrayValue);
        job_exp = user["previous_exp"];

        for (Json::ValueConstIterator jobItr = job_exp.begin(); jobItr != job_exp.end(); jobItr++) {
            Json::Value position = (*jobItr)["position"];

            if (position["name"].asString() == job) {
                aux_result.append(user);
                break;
            }
        }
    }
    result.swapPayload(aux_result);
}

void UsersDB::filter_skill(Json::Value &result, string skill) {
    if (skill.empty())
        return;
    Json::Value aux_result(Json::arrayValue);

    for (Json::ValueConstIterator itr = result.begin(); itr != result.end(); itr++) {
        Json::Value user = *itr;

//        std::cerr << "\nBUSCANDO " << skill;
//        std::cerr << "\nSKILLS --> " << user["skills"];
        Json::Value skills(Json::arrayValue);
        skills = user["skills"];

        for (Json::ValueConstIterator jobItr = skills.begin(); jobItr != skills.end(); jobItr++) {
            Json::Value json_skill = (*jobItr);

            if (json_skill["name"].asString() == skill) {
                aux_result.append(user);
                break;
            }
        }
    }
    result.swapPayload(aux_result);
}

void UsersDB::sort_by(Json::Value &result, string sorting) {
    if (sorting.empty())
        return;

    Json::Value aux_result(Json::arrayValue);
    std::vector<std::pair<int, Json::Value> > ordered;

    for (Json::ValueConstIterator itr = result.begin(); itr != result.end(); itr++) {
        Json::Value user = *itr;

//        std::cerr << "\nORDENANDO";
//        std::cerr << "\n" << sorting << " --> " << user[sorting];

        int cant = user[sorting].size();
        std::pair<int, Json::Value> pair = std::make_pair(cant, user);
        ordered.push_back(pair);
    }

    std::sort(ordered.begin(), ordered.end(),
              [](const std::pair<int, Json::Value> &left, const std::pair<int, Json::Value> &right) {
                  return left.first > right.first;
              });

    for (std::pair<int, const Json::Value &> aux : ordered) {
        aux_result.append(aux.second);
    }

    result.swapPayload(aux_result);
}

void UsersDB::top_k(Json::Value &result, int n) {
    if (n == 0)
        n = 10;

    if (result.size() < n)
        return;

    result.resize(n);
}

void UsersDB::sort_by_distance(Json::Value &result, std::string caller_coordenates) {
    Json::Value aux_result(Json::arrayValue);
    std::vector<std::pair<int, Json::Value> > ordered;

    for (Json::ValueConstIterator itr = result.begin(); itr != result.end(); itr++) {
        Json::Value user = *itr;

        std::cerr << "\nORDENANDO POR DISTANCIA";

        double dist = calcularDistancia(user["coordenates"].asString(),caller_coordenates);

        std::pair<double, Json::Value> pair = std::make_pair(dist, user);
        ordered.push_back(pair);
    }

    //ordeno de mas cerca a mas lejos
    std::sort(ordered.begin(), ordered.end(),
              [](const std::pair<double, Json::Value> &left, const std::pair<double, Json::Value> &right) {
                  return left.first < right.first;
              });

    for (std::pair<double, const Json::Value &> aux : ordered) {
        aux_result.append(aux.second);
    }

    result.swapPayload(aux_result);
}

double UsersDB::calcularDistancia(string coord_user, string callerCoordenates) {
    double lat1, long1, lat2, long2;
    getCoordinates(coord_user, &lat1, &long1);
    getCoordinates(callerCoordenates, &lat2, &long2);

    return distanceEarth(lat1, long1, lat2, long2);
}

