//
// Created by ale on 29/11/16.
//

#ifndef JOBIFY_APPSERVER_IMAGESDB_H
#define JOBIFY_APPSERVER_IMAGESDB_H


#include "DB.h"

class ImagesDB: public DB {

public:
    ImagesDB(const std::string &db_name);

    virtual ~ImagesDB();

    bool add_image(const std::string &username, Json::Value &image);
    bool update_image(const std::string &username, Json::Value &image);
    Json::Value get_image(const std::string &username);
    bool delete_image(const std::string &username);
};





#endif //JOBIFY_APPSERVER_IMAGESDB_H
