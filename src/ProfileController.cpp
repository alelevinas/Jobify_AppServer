//
// Created by ale on 15/09/16.
//
#include <json/json.h>

#include "ProfileController.h"

using Json::Value;
using namespace Mongoose;


ProfileController::ProfileController(DatabaseManager* db) : db(db) {

}


void ProfileController::getUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string username = htmlEntities(request.get("username",""));

    Json::Value user = db->get_user(username);

    if (user["username"] != username)
        response["Error"] = "Hubo un error nro 435684";

    response["response"] = user;
}


void ProfileController::setup() {

    // putting all the urls into "/api"
    setPrefix("/user");

    // Hello demo
    addRouteResponse("GET", "", ProfileController, getUserRequest, JsonResponse);

}

