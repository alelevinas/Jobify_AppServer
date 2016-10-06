//
// Created by ale on 4/10/16.
//
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <iostream>
#include <sstream>
#include <exceptions/KeyDoesntExistException.h>
#include <exceptions/KeyAlreadyExistsException.h>
#include <exceptions/TokenDoesntExistException.h>
#include "SessionManager.h"
#include "cryptopp/base64.h"
#include "cryptopp/md5.h"
#include "cryptopp/hex.h"


SessionManager::SessionManager(DatabaseManager *dbManager) : dbManager(dbManager) {}

SessionManager::~SessionManager() {

}

std::string SessionManager::get_timestamp_now() {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime (&rawtime);

    return std::string(asctime (timeinfo));

}

std::string SessionManager::get_hashed_usr_pass(std::string &username, std::string &password, std::string &timestamp) {
    std::string usr_pass = username + ":" + password + ":" + timestamp ;

    using namespace CryptoPP;
    std::string encodedUsrNPass;
    StringSource(usr_pass, true, new Base64Encoder(new StringSink(encodedUsrNPass)));

    std::cout << "Base64: " << encodedUsrNPass << std::endl;

    CryptoPP::Weak::MD5 hash;
    std::string digest;

    using namespace CryptoPP;
    StringSource s(encodedUsrNPass, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
    return digest;
}

std::string SessionManager::add_session(std::string &username, std::string &password) {




    /*fijarse si ya esta, si esta renovar el timestamp, si no esta agregar*/

    std::string timestamp = get_timestamp_now();

    std::string hashed = get_hashed_usr_pass(username, password,timestamp);

    Json::Value user_timestamp;
    user_timestamp["username"] = username;
    user_timestamp["timestamp"] = timestamp;

    try {
        dbManager->add_session(hashed, user_timestamp);
    } catch (KeyAlreadyExistsException e) {
        //return "";
        throw e;
    }

    return hashed;
}

bool SessionManager::has_expired(std::string &token) {
    try {
        Json::Value session = dbManager->get_session(token);
        std::string timestamp = session["timestamp"].asString();

        return this->timestamp_has_expired(timestamp);
    } catch (KeyDoesntExistException e) {
        //LOG e.what()
        return false;
    }
}

bool SessionManager::timestamp_has_expired(std::string &timestamp) {
    std::stringstream ss(timestamp);

    std::string wday, month, mday, hour, min, seg, year;

    ss >> wday;
    ss >> month;
    ss >> mday;
    std::getline(ss, hour, ':');
    std::getline(ss, min, ':');
    ss >> seg;
    ss >> year;

    struct tm _then = {0};
    _then.tm_hour = stoi(hour) ;
    _then.tm_min = stoi(min);
    _then.tm_sec = stoi(seg);
    _then.tm_year = stoi(year);
    _then.tm_mon = stoi(month);
    _then.tm_mday = stoi(mday);

    time_t now = time(NULL);

    double diff = difftime(now, mktime(&_then));

    return diff > 3600; //2h
}

std::string SessionManager::get_username(std::string &token) {
    try {
        Json::Value session = dbManager->get_session(token);
        return session["username"].asString();
    } catch (KeyDoesntExistException e) {
        throw TokenDoesntExistException();
    }
}