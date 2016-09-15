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

void ProfileController::postUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string username = request.get("username","");

    std::string json_user = request.getData(); //el body

    cerr << "POST USER: username=" << username;
    cerr << "\nPOST DATA: " << json_user;

    Json::Reader reader;
    Json::Value user;
    bool parsingSuccessful = reader.parse( json_user, user);
    if (!parsingSuccessful) {
        response["Error"] = "Hubo un error de parseo nro 435684"; //levantar excepcion??
        return;
    }

    if (!db->add_user(username,user))
        response["Error"] = "El usuario ya existe... o la bd no pudo guardarlo nro 435684";

    response["response"] = "Ok";
}



void ProfileController::setup() {

    // putting all the urls into "/api"
    setPrefix("/user");

    // Hello demo
    addRouteResponse("GET", "", ProfileController, getUserRequest, JsonResponse);
    addRouteResponse("POST","",ProfileController,postUserRequest,JsonResponse);

}

