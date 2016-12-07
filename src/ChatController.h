//
// Created by nicolas on 26/11/16.
//

#ifndef JOBIFY_APPSERVER_CHATCONTROLLER_H
#define JOBIFY_APPSERVER_CHATCONTROLLER_H

#include <JsonController.h>
#include "DatabaseManager.h"
#include "SessionManager.h"
#include "NotificationSender.h"

/**
 * Controller that manages the chat related api endpoints. Every method recieves a
 * Moongose::Request with the request data and a Mongoose::JsonResponse
 * to store the response json.
 */
class ChatController : public Mongoose::JsonController {

    DatabaseManager* db;
    SessionManager* sessionManager;
    NotificationSender *notSender;

public:
    ChatController(DatabaseManager *pManager, SessionManager *pSessionManager, std::string fireServerToken);

    void setup();

    /**
     * Endpoint to send a chat message to another user
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void postUserChatRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Endpoint to get chat messages with another user
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getUserChatsRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Endpoint to delete a conversation with another user
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void deleteConversationRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Endpoint to delete a chat message with another user
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void deleteMessageRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);
};


#endif //JOBIFY_APPSERVER_CHATCONTROLLER_H
