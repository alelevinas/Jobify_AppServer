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
};


#endif //JOBIFY_APPSERVER_PROFILECONTROLLER_H
