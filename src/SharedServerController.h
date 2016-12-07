//
// Created by nicolas on 30/11/16.
//

#ifndef JOBIFY_APPSERVER_SHAREDSERVERCONTROLLER_H
#define JOBIFY_APPSERVER_SHAREDSERVERCONTROLLER_H


#include <ClientSharedServer.h>
#include <SessionManager.h>
#include <JsonController.h>

/**
 * Controller that manages the shared Server common data. The connection to the shared server is made
 * through the ClientSharedServer.
 */
class SharedServerController : public Mongoose::JsonController {
private:
    ClientSharedServer* client;
    SessionManager* sessionManager;
public:
    SharedServerController(ClientSharedServer *clientSharedServer, SessionManager *sessionManager);

    void setup();

    /**
     * Gets the job positions from the shared server and responds them
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getJobPosRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Gets the skills from the shared server and responds them
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getSkillsRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Gets the categories from the shared server and responds them
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getCategoriesRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
};


#endif //JOBIFY_APPSERVER_SHAREDSERVERCONTROLLER_H
