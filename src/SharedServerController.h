//
// Created by nicolas on 30/11/16.
//

#ifndef JOBIFY_APPSERVER_SHAREDSERVERCONTROLLER_H
#define JOBIFY_APPSERVER_SHAREDSERVERCONTROLLER_H


#include <ClientSharedServer.h>
#include <SessionManager.h>
#include <JsonController.h>

class SharedServerController : public Mongoose::JsonController {
private:
    ClientSharedServer* client;
    SessionManager* sessionManager;
public:
    SharedServerController(ClientSharedServer *clientSharedServer, SessionManager *sessionManager);

    void setup();

    void getJobPosRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    void getSkillsRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    void getCategoriesRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
};


#endif //JOBIFY_APPSERVER_SHAREDSERVERCONTROLLER_H
