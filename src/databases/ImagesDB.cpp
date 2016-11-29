//
// Created by ale on 29/11/16.
//

#include "ImagesDB.h"

ImagesDB::ImagesDB(const std::string &db_name) : DB(db_name) {}

ImagesDB::~ImagesDB() {

}


bool ImagesDB::add_image(const std::string &username, Json::Value &image) {
    return this->add(username, image);
}

Json::Value ImagesDB::get_image(const std::string &username) {
    return this->get(username);
}

bool ImagesDB::update_image(const std::string &key, Json::Value &value) {
    return this->update(key, value);

}

bool ImagesDB::delete_image(const std::string &username) {
    return this->delete_key(username);
}