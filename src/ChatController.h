//
// Created by nicolas on 26/11/16.
//

#ifndef JOBIFY_APPSERVER_CHATCONTROLLER_H
#define JOBIFY_APPSERVER_CHATCONTROLLER_H

#include <JsonController.h>
#include "DatabaseManager.h"
#include "SessionManager.h"
#include "NotificationSender.h"

class ChatController : public Mongoose::JsonController {

    DatabaseManager* db;
    SessionManager* sessionManager;
    NotificationSender* notSender;
public:
    ChatController(DatabaseManager *pManager, SessionManager *pSessionManager, std::string fireServerToken);

    void setup();

    void postUserChatRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    void getUserChatsRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    void deleteConversationRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    void deleteMessageRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
};


#endif //JOBIFY_APPSERVER_CHATCONTROLLER_H
