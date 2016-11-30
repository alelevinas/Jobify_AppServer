//
// Created by nicolas on 29/11/16.
//

#include "ClientSharedServer.h"
#include <iostream>

ClientSharedServer::ClientSharedServer(std::string url, int port) {
    std::stringstream ss;
    ss << url << ":" << port;
    this->url = ss.str();
}

std::string ClientSharedServer::getJobPositions() {
    std::string sUrlJobPos = this->url + "/" + JOB_POSITIONS;
    RestClient::Response r;
    r = RestClient::get(sUrlJobPos);
    return r.body;
}

bool ClientSharedServer::getNamesJobPositions(Json::Value *names) {
    std::string sUrlJobPos = this->url + "/" + JOB_POSITIONS;
    RestClient::Response r;
    r = RestClient::get(sUrlJobPos);

    Json::Reader reader;
    Json::Value job_position;
    bool parsingSuccessful = reader.parse(r.body, job_position);
    if (!parsingSuccessful) {
        return false;
    }

    Json::Value newArrayValue(Json::arrayValue);
    for (const Json::Value& jobposition : job_position["job_positions"]) {
        newArrayValue.append(jobposition["name"]);
    }

    *names = newArrayValue;
    return true;
}

bool ClientSharedServer::getNamesSkills(Json::Value *names) {
    std::string sUrlSkills = this->url + "/" + SKILLS;
    RestClient::Response r;
    r = RestClient::get(sUrlSkills);

    Json::Reader reader;
    Json::Value skills;
    bool parsingSuccessful = reader.parse(r.body, skills);
    if (!parsingSuccessful) {
        return false;
    }

    Json::Value newArrayValue(Json::arrayValue);
    for (const Json::Value& skill : skills["skills"]) {
        newArrayValue.append(skill["name"]);
    }

    *names = newArrayValue;
    return true;
}

bool ClientSharedServer::getNamesCategories(Json::Value *names) {
    std::string sUrlCategories = this->url + "/" + CATEGORIES;
    RestClient::Response r;
    r = RestClient::get(sUrlCategories);

    Json::Reader reader;
    Json::Value categories;
    bool parsingSuccessful = reader.parse(r.body, categories);
    if (!parsingSuccessful) {
        return false;
    }

    Json::Value newArrayValue(Json::arrayValue);
    for (const Json::Value& skill : categories["categories"]) {
        newArrayValue.append(skill["name"]);
    }

    *names = newArrayValue;
    return true;
}