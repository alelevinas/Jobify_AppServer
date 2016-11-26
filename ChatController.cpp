//
// Created by nicolas on 26/11/16.
//

#include "ChatController.h"

using Json::Value;
using namespace Mongoose;


ChatController::ChatController(DatabaseManager *pManager, SessionManager *pSessionManager)
        : db(db), sessionManager(sessionManager) {}

void ChatController::setup() {
    addRouteResponse("POST", "/chat", ChatController, postUserChatRequest, JsonResponse);
    addRouteResponse("GET", "/chat", ChatController, getUserChatRequest, JsonResponse);
}

void ChatController::postUserChatRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {

}

void ChatController::getUserChatRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {

}
