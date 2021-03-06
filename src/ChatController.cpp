//
// Created by nicolas on 26/11/16.
//
// TOKEN mocca@gmail.com = bW9jY2FAZ21haWwuY29tOjEyMzQ=

#include <exceptions/TokenInvalidException.h>
#include <log/easylogging++.h>

#include "ChatController.h"
#include "exceptions/KeyDoesntExistException.h"
#include "ApiError.h"

using Json::Value;
using namespace Mongoose;


ChatController::ChatController(DatabaseManager *db, SessionManager *sessionManager, std::string fireServerToken)
        : db(db), sessionManager(sessionManager) {
    notSender = new NotificationSender(fireServerToken);
}

void ChatController::setup() {
    addRouteResponse("GET", "/chats", ChatController, getUserChatsRequest, JsonResponse);
    addRouteResponse("POST", "/chats", ChatController, postUserChatRequest, JsonResponse);
    addRouteResponse("DELETE", "/chats", ChatController, deleteConversationRequest, JsonResponse);
    addRouteResponse("DELETE", "/messages", ChatController, deleteMessageRequest, JsonResponse);
}

void ChatController::getUserChatsRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    std::string username;

    try {
        username = sessionManager->get_username(token);
        std::string username2 = request.get("user","");

        LOG(INFO) << "USER CHAT GET REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username
                  <<"\t\tConversacion con: " << username2 << std::endl;

        Json::Value user = db->get_user(username);
        Json::Value conversation;

        if (user["username"] != username) {
            ApiError::setError(response,500,"Internal server error");
        } else {
            bool ok = false;
            if (username2 == "") {
                ok = db->get_convs(username, &conversation);
            } else {
                Json::Value user2 = db->get_user(username2);
                if (user2["username"] != username2) {
                    ApiError::setError(response, 500, "Internal server error");
                } else {
                    ok = db->get_conv(username, username2, &conversation);
                }
            }
            if (!ok) {
                ApiError::setError(response,500,"Internal server error");
            } else {
                response[STATUS] = SUCCES;
                if (username2 == "") {
                    response[DATA] = conversation["chats"];
                } else {
                    response[DATA] = conversation;
                }
            }
        }
    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"Token invalido");
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,500,"Usuario inexistente");
    }
    LOG(INFO) << "USER CHAT GET RESPONSE:\n"
              << "\t\tUser: " << username << "\n"
              << "\t\tResponse: " << response
              << std::endl;
}

void ChatController::postUserChatRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    std::string username;
    std::string data;

    try {
        username = sessionManager->get_username(token);

        data = request.getData();
        Json::Reader reader;
        Json::Value content;
        bool parsingSuccessful = reader.parse(data, content);
        if (!parsingSuccessful) {
            ApiError::setError(response,410,"Wrong JSON");  // TODO agregar a la API documentation
        }
        std::string msgTo = request.get("user","");
        std::string message = content.get("text","").asString();

        LOG(INFO) << "USER CHAT POST REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username
                <<"\t\tMensaje a: " << msgTo 
                <<"\t\tMensaje: " << message << "-------" << std::endl;

        Json::Value user = db->get_user(username);
        Json::Value user2 = db->get_user(msgTo);

        if ((user["username"] != username) || (user2["username"] != msgTo)){
            ApiError::setError(response,500,"Internal server error");
        } else {
            if (!db->add_msg(username,user["name"].asString(), msgTo, message)) {
                ApiError::setError(response,500,"Internal server error");
            } else {
                response[STATUS] = SUCCES;
                response[DATA] = OKK;
                //enviar notificacion
                notSender->send_notification(username, user2["firebase_token"].asString(), message,
                                             user["name"].asString());
            }
        }
    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"Token invalido");
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,500,"Usuario inexistente");
    }
    LOG(INFO) << "USER CHAT POST RESPONSE:\n"
              << "\t\tUser: " << username << "\n"
              << "\t\tResponse: " << response
              << std::endl;
}

void ChatController::deleteConversationRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    std::string username;

    try {
        username = sessionManager->get_username(token);

        std::string username2 = request.get("user","");

        LOG(INFO) << "USER CHAT DELETE REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username
                  <<"\t\tConversacion con: " << username2 << std::endl;

        Json::Value user = db->get_user(username);
        Json::Value user2 = db->get_user(username2);

        if ((user["username"] != username) || (user2["username"] != username2)) {
            ApiError::setError(response,500,"Internal server error");
        } else {
            if (!db->delete_conv(username, username2)) {
                ApiError::setError(response, 500, "Internal server error");
            } else {
                response[STATUS] = SUCCES;
                response[DATA] = OKK;
            }
        }
    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"Token invalido");
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,500,"Usuario inexistente");
    }
    LOG(INFO) << "USER CHAT DELETE RESPONSE:\n"
              << "\t\tUser: " << username << "\n"
              << "\t\tResponse: " << response
              << std::endl;
}

void ChatController::deleteMessageRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    std::string username;
    std::string data;

    try {
        username = sessionManager->get_username(token);

        data = request.getData();
        Json::Reader reader;
        Json::Value content;
        bool parsingSuccessful = reader.parse(data, content);
        if (!parsingSuccessful) {
            ApiError::setError(response,410,"Wrong JSON");
        }
        std::string username2 = request.get("user","");
        std::string idMessage = content.get("id",Json::Value("")).asString();

        LOG(INFO) << "USER MESSAGE DELETE REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username
                  <<"\t\tConversacion con: " << username2
                  <<"\t\tMensaje ID: " << idMessage << "-------" << std::endl;

        Json::Value user = db->get_user(username);
        Json::Value user2 = db->get_user(username2);

        if ((user["username"] != username) || (user2["username"] != username2)){
            ApiError::setError(response,500,"Internal server error");
        } else {
            if (!db->delete_msg(username, username2, idMessage)) {
                ApiError::setError(response,500,"Mensaje no encontrado");
            } else {
                response[STATUS] = SUCCES;
                response[DATA] = OKK;
            }
        }
    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"Token invalido");
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,500,"Usuario inexistente");
    }
    LOG(INFO) << "USER MESSAGE DELETE RESPONSE:\n"
              << "\t\tUser: " << username << "\n"
              << "\t\tResponse: " << response
              << std::endl;
}
