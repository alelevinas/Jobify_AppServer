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
}

void SharedServerController::getSkillsRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {

}

void SharedServerController::getCategoriesRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {

}