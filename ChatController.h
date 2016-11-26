//
// Created by nicolas on 26/11/16.
//

#ifndef JOBIFY_APPSERVER_CHATCONTROLLER_H
#define JOBIFY_APPSERVER_CHATCONTROLLER_H

#include <JsonController.h>
#include "DatabaseManager.h"
#include "SessionManager.h"

class ChatController : public Mongoose::JsonController {

    DatabaseManager* db;
    SessionManager* sessionManager;
public:
    ChatController(DatabaseManager *pManager, SessionManager *pSessionManager);

    void setup();

    void getUserChatRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
    void postUserChatRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
};


#endif //JOBIFY_APPSERVER_CHATCONTROLLER_H
