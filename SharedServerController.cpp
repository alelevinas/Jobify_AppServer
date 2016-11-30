//
// Created by nicolas on 30/11/16.
//

#include <log/easylogging++.h>
#include <ApiError.h>
#include <exceptions/TokenInvalidException.h>
#include "SharedServerController.h"

using namespace Mongoose;

SharedServerController::SharedServerController(ClientSharedServer *clientSharedServer, SessionManager *sessionManager)
        : client(clientSharedServer), sessionManager(sessionManager){

}

void SharedServerController::setup() {
    addRouteResponse("GET", "/job_positions", SharedServerController, getJobPosRequest, JsonResponse);
    addRouteResponse("GET", "/skills", SharedServerController, getSkillsRequest, JsonResponse);
    addRouteResponse("GET", "/categories", SharedServerController, getCategoriesRequest, JsonResponse);
}

void SharedServerController::getJobPosRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    std::string username;

    try {
        username = sessionManager->get_username(token);

        LOG(INFO) << "JOBPOSITIONs GET REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << std::endl;

        Json::Value job_positions;
        bool ok = client->getNamesJobPositions(&job_positions);
        if(ok) {
            response[STATUS] = SUCCES;
            response[DATA] = job_positions;
        } else {
            ApiError::setError(response,500,"Internal server error");
        }
    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"Token invalido");
    }
    LOG(INFO) << "JOBPOSITIONs GET REQUEST:\n"
              << "\t\tUser: " << username << "\n"
              << "\t\tResponse: " << response
              << std::endl;
}

void SharedServerController::getSkillsRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {

}

void SharedServerController::getCategoriesRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {

}