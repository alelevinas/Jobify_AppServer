//
// Created by ale on 15/09/16.
//
#include <json/json.h>
#include <exceptions/UserAlreadyExistsException.h>

#include "ProfileController.h"
#include "exceptions/UserDoesntExistException.h"

using Json::Value;
using namespace Mongoose;


ProfileController::ProfileController(DatabaseManager* db) : db(db) {

}


void ProfileController::getUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string username = htmlEntities(request.get("username",""));

    try {
        Json::Value user = db->get_user(username);

        cerr << "\nGET USER: username=" << username;
        cerr << "\nJSON user: " << user;

        if (user["username"] != username) {
            response["Error"] = "Hubo un error nro 435684";
            return;
        }

        response["response"] = user;
    }catch (UserDoesntExistException& e) {
        response["Error"] = "No existe tal usuario"; //MAL!!!
        return;
    }
}

void ProfileController::postUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string username = request.get("username","");

    std::string json_user = request.getData(); //el body

    cerr << "\nPOST USER: username=" << username;
    cerr << "\nPOST DATA: " << json_user;

    Json::Reader reader;
    Json::Value user;
    bool parsingSuccessful = reader.parse( json_user, user);
    if (!parsingSuccessful) {
        response["Error"] = "Hubo un error de parseo del json nro 435684"; //levantar excepcion??
        return;
    }

    try {
        if (!db->add_user(username, user))
            response["Error"] = "Server error 435684";

        response["response"] = "Ok";

    } catch (UserAlreadyExistsException& e) {
            response["Error"] = "El usuario ya existe...";
    }


}



void ProfileController::setup() {

    // putting all the urls into "/api"
    setPrefix("/users"); //para el GET users/username para uno
                        // para el GET users/ para devolver todos
                        // para el POST users/ para postear


    // Hello demo
    addRouteResponse("GET", "", ProfileController, getUserRequest, JsonResponse);
    addRouteResponse("POST","",ProfileController,postUserRequest,JsonResponse);

}

/*
 * pa'l checkpoint 2:
 * la documentacion: usar el que nos dieron ellos para el API REST
 * un diagrama de como funciona el programa a grandes rasgos
 * pruebas en python a la API REST
 * modelo de datos de leveldb --> el json
 * TEST+DOCUMENTACION+FUNCIONALIDAD --> clave!
 *
 *
 */