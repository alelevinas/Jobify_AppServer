//
// Created by ale on 15/09/16.
//

#ifndef JOBIFY_APPSERVER_PROFILECONTROLLER_H
#define JOBIFY_APPSERVER_PROFILECONTROLLER_H


#include <JsonController.h>
#include "DatabaseManager.h"

class ProfileController : public Mongoose::JsonController{

    DatabaseManager* db;
public:
    ProfileController(DatabaseManager* db);

    void setup();

    //Metodos
    void getUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
    void postUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
    void updateUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    void getLogin(Mongoose::Request &request, Mongoose::JsonResponse &response);

private:
    void decodeAuth(std::string &b64_auth, std::string &usr, std::string &pass);
};


#endif //JOBIFY_APPSERVER_PROFILECONTROLLER_H
