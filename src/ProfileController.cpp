//
// Created by ale on 15/09/16.
//
#include <json/json.h>
#include <exceptions/KeyAlreadyExistsException.h>
#include <cryptopp/base64.h>
#include <exceptions/TokenInvalidException.h>

#include "ProfileController.h"
#include "exceptions/KeyDoesntExistException.h"

using Json::Value;
using namespace Mongoose;


ProfileController::ProfileController(DatabaseManager *db, SessionManager *sessionManager)
        : db(db), sessionManager(sessionManager) {}

void ProfileController::decodeAuth(std::string &usr_pass_b64, std::string &usr, std::string &pass) {
    cerr << "\nAuthorization: " << usr_pass_b64 << std::endl;
    std::stringstream ss(usr_pass_b64);
    ss >> usr_pass_b64; //"=Basic" chunck
    ss >> usr_pass_b64; //base64
    cerr << "\nBasic Authorization: (just b64)" << usr_pass_b64 << std::endl;

    using namespace CryptoPP;
    string decodedUsrNPass;
    StringSource(usr_pass_b64, true, new Base64Decoder(new StringSink(decodedUsrNPass)));

    std::cerr << "\ndecoded Base64: " << decodedUsrNPass << std::endl;

    ss.flush();


    std::stringstream ss2(decodedUsrNPass);
    std::getline(ss2, usr, ':');
    ss2 >> pass;
}


void ProfileController::getUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

        cerr << " es del usuario: " << username;

        Json::Value user = db->get_user(username);

        cerr << "\nGET USER: username=" << username;
        cerr << "\nJSON user: " << user;

        if (user["username"] != username) {
            response["Error"] = "Hubo un error nro 435684";
            return;
        }

        response.swap(user);
    } catch (TokenInvalidException &e) {
        response["Error"] = "token invalido"; //MAL!!!
        return;
    } catch (KeyDoesntExistException &e) {
        response["Error"] = "No existe tal usuario"; //MAL!!!
        return;
    }
}

void ProfileController::getUsersRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

        cerr << " es del usuario: " << username;

        response["users"] = db->get_users();
    } catch (TokenInvalidException &e) {
        response["Error"] = "token invalido"; //MAL token!!!
    }
}

void ProfileController::postUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string usr_pass_b64 = request.getHeaderKeyValue("Authorization");

    std::string usr, pass;
    this->decodeAuth(usr_pass_b64, usr, pass);

    cerr << "\nauth_username: " << usr
         << "\nauth_pass: " << pass << std::endl;

    std::string json_user = request.getData(); //el body

    cerr << "\nDATA: " << json_user;

    Json::Reader reader;
    Json::Value user;
    bool parsingSuccessful = reader.parse(json_user, user);
    if (!parsingSuccessful) {
        response["Error"] = "Hubo un error de parseo del json nro 435684"; //levantar excepcion??
        return;
    }

    try {
        if (!db->add_account(usr, pass))
            response["Error"] = "Server error 435684: db error";

        if (!db->add_user(usr, user))
            response["Error"] = "Server error 435684: db error";

        response["token"] = sessionManager->add_session(usr, pass);

    } catch (KeyAlreadyExistsException &e) {
        response["Error"] = "El username ya existe...";
    }
}

void ProfileController::updateUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

        cerr << " es del usuario: " << username;
        db->get_user(username);

        std::string json_user = request.getData(); //el body

        Json::Reader reader;
        Json::Value edited_user;
        bool parsingSuccessful = reader.parse(json_user, edited_user);
        if (!parsingSuccessful) {
            response["Error"] = "Hubo un error de parseo del json nro 435684";  //levantar excepcion??
            return;
        }

        db->edit_user(username, edited_user);
        response["response"] = "ok";
    } catch (KeyDoesntExistException &e) {
        response["Error"] = "No existe tal usuario"; //MAL!!!
        return;
    } catch (TokenInvalidException &e) {
        response["Error"] = "token invalido"; //MAL token!!!
    }
}

void ProfileController::deleteUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

        cerr << " es del usuario: " << username;

        if (!db->delete_user(username))
            response["response"] = "Error en la db";

        db->delete_session(token);
        response["response"] = "ok";
    } catch (KeyDoesntExistException &e) {
        response["Error"] = "No existe tal usuario"; //MAL!!!
        return;
    } catch (TokenInvalidException &e) {
        response["Error"] = "token invalido"; //MAL token!!!
    }
}

void ProfileController::getLogin(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string usr_pass_b64 = request.getHeaderKeyValue("Authorization");

    std::string usr, pass;
    this->decodeAuth(usr_pass_b64, usr, pass);

    try {
        if (db->is_correct(usr, pass))
            response["token"] = sessionManager->add_session(usr, pass);
    } catch (KeyAlreadyExistsException &e) {
        response["Error"] = "Ya esta logeado...";
    } catch (KeyDoesntExistException &e) {
        response["Error"] = "No existe tal usuario";
    }
}


void ProfileController::setup() {

    // putting all the urls into "/api"
    //setPrefix("/users"); //para el GET users/username para uno
    // para el GET users/ para devolver todos
    // para el POST users/ para postear

    // rutasss
    addRouteResponse("POST", "/signup", ProfileController, postUserRequest, JsonResponse);
    addRouteResponse("GET", "/login", ProfileController, getLogin, JsonResponse);

    addRouteResponse("GET", "/users", ProfileController, getUsersRequest, JsonResponse);
    addRouteResponse("GET", "/users/me", ProfileController, getUserRequest, JsonResponse);
    addRouteResponse("POST", "/users/me", ProfileController, updateUserRequest, JsonResponse);
    addRouteResponse("DELETE", "/users/me", ProfileController, deleteUserRequest, JsonResponse);


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