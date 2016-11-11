//
// Created by ale on 15/09/16.
//
#include <json/json.h>
#include <exceptions/KeyAlreadyExistsException.h>
#include <cryptopp/base64.h>
#include <exceptions/TokenInvalidException.h>
#include <log/easylogging++.h>

#include "ProfileController.h"
#include "exceptions/KeyDoesntExistException.h"
#include "ApiError.h"
using Json::Value;
using namespace Mongoose;

// #define STATUS "status"
// #define SUCCES "succes"
// #define ERROR "error"
// #define DATA "data"


ProfileController::ProfileController(DatabaseManager *db, SessionManager *sessionManager)
        : db(db), sessionManager(sessionManager) {}

void ProfileController::decodeAuth(std::string &usr_pass_b64, std::string &usr, std::string &pass) {
    // cerr << "\nAuthorization: " << usr_pass_b64 << std::endl;
    std::stringstream ss(usr_pass_b64);
    ss >> usr_pass_b64;  // "=Basic" chunck
    ss >> usr_pass_b64; //base64
    //cerr << "\nBasic Authorization: (just b64)" << usr_pass_b64 << std::endl;

    using namespace CryptoPP;
    string decodedUsrNPass;
    StringSource(usr_pass_b64, true, new Base64Decoder(new StringSink(decodedUsrNPass)));

    //std::cerr << "\ndecoded Base64: " << decodedUsrNPass << std::endl;

    ss.flush();


    std::stringstream ss2(decodedUsrNPass);
    std::getline(ss2, usr, ':');
    ss2 >> pass;
}


void ProfileController::getUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    cerr << "\ntoken recibido " << token;

    std::string username;
    try {
        username = sessionManager->get_username(token);

        LOG(INFO) << "USER GET REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username
                  << std::endl;

        cerr << " es del usuario: " << username;

        Json::Value user = db->get_user(username);

        cerr << "\nGET USER: username=" << username;
        cerr << "\nJSON user: " << user;

        if (user["username"] != username) {
            ApiError::setError(response,500,"Internal server error");
        } else {
            response[STATUS] = SUCCES;
            response[DATA] = user;
        }

    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"token invalido");
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,500,"Internal server error");
    }
    LOG(INFO) << "USER GET RESPONSE:\n"
              << "\t\tUser: " << username << "\n"
              << "\t\tResponse: " << response
              << std::endl;
}

void ProfileController::getUsersRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    //std::string token = request.getHeaderKeyValue("Token");
    //cerr << "\ntoken recibido " << token;

    try {
        //std::string username = sessionManager->get_username(token);

        //cerr << " es del usuario: " << username;

        LOG(INFO) << "USERS GET REQUEST:\n"
                  //<< "\t\tHeader Token: " << token << "\n"
                  //<< "\t\tUser: " << username
                  << std::endl;

        std::string json_users = db->get_users();
        std::cerr << json_users;

        Json::Reader reader;
        Json::Value users;
        bool ok = true;
        bool parsingSuccessful = reader.parse(json_users, users);
        if (!parsingSuccessful) {
            ok = false;
            ApiError::setError(response,500,"Internal server error");
        }

        response[STATUS] = SUCCES;
        response[DATA] = users;


    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"token invalido");
    }
    LOG(INFO) << "USERS RESPONSE:\n"
              << "\t\tResponse: " << response
              << std::endl;
}

//signup
void ProfileController::postUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string usr_pass_b64 = request.getHeaderKeyValue("Authorization");
    std::string usr, pass;
    this->decodeAuth(usr_pass_b64, usr, pass);


    std::string json_user = request.getData(); //el body

    LOG(INFO) << "SIGNUP REQUEST:\n"
              << "\t\tHeader Authorization: " << usr_pass_b64 << "\n"
              << "\t\tUser: " << usr << " Password: " << pass << "\n"
              << "\t\tData: " << json_user
              << std::endl;

    Json::Reader reader;
    Json::Value user;
    bool ok = true;
    bool parsingSuccessful = reader.parse(json_user, user);
    if (!parsingSuccessful) {
        ok = false;
        ApiError::setError(response,410,"Wrong JSON");  // TODO agregar a la API documentation
    }
    user["username"] = usr;
    if(user["chats"].empty())
        user["chats"] = Json::Value(Json::arrayValue);
    if(user["contacts"].empty())
        user["contacts"] = Json::Value(Json::arrayValue);
    if(user["recommended_by"].empty())
        user["recommended_by"] = Json::Value(Json::arrayValue);
    try {
        if (ok && !db->add_account(usr, pass)) {
            ok = false;
            ApiError::setError(response,500,"Internal server error");
        }
        if (ok && !db->add_user(usr, user)) {
            ok = false;
            //TODO remove_account que nisiquiera existe
            ApiError::setError(response,500,"Internal server error");
        }
        if (ok) {
            std::string token = sessionManager->add_session(usr, pass);;
            if (token == "") {
                //TODO remove account and user
                ApiError::setError(response,500,"Internal server error");
            } else {
                response[STATUS] = SUCCES;
                response[DATA]["token"] = token;
            }
        }
    } catch (KeyAlreadyExistsException &e) {
        ApiError::setError(response,409,"Email already in use");
    }
    LOG(INFO) << "SIGNUP RESPONSE:\n"
              << "\t\t" << response
              << std::endl;
}

void ProfileController::updateUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);


        cerr << " es del usuario: " << username;

        std::string json_user = request.getData(); //el body

        LOG(INFO) << "UPDATE USER REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << "\n"
                  << "\t\tData: " << json_user
                  << std::endl;

        db->get_user(username); //solo para ver si salta la exception

        bool ok = true;
        Json::Reader reader;
        Json::Value edited_user;
        bool parsingSuccessful = reader.parse(json_user, edited_user);
        if (!parsingSuccessful) {
            ApiError::setError(response,410,"Wrong JSON");  // TODO agregar a la API documentation
            ok = false;
        }
        if (ok && db->edit_user(username, edited_user)) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        } else {
            ApiError::setError(response,500,"Internal server error");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,500,"Internal server error");
        return;
    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"token invalido");
    }
    LOG(INFO) << "UPDATE USER RESPONSE:\n"
              << "\t\tResponse: " << response
              << std::endl;
}

void ProfileController::deleteUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

        LOG(INFO) << "DELETE USER REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username;

        cerr << " es del usuario: " << username;

        bool ok = true;
        if (!db->delete_user(username)) {
            ApiError::setError(response,500,"Internal server error");
            ok = false;
        }
        if (!db->delete_session(token)) {
            // volver a poner el usuario?
            ApiError::setError(response,500,"Internal server error");
            ok = false;
        } //todo delete account
        if (ok) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,500,"Internal server error");
    } catch (TokenInvalidException &e) {
        ApiError::setError(response,501,"token invalido");
    }
    LOG(INFO) << "DELETE USER RESPONSE:\n"
              << "\t\tResponse: " << response
              << std::endl;
}

void ProfileController::getLogin(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string usr_pass_b64 = request.getHeaderKeyValue("Authorization");
    if (usr_pass_b64 == "") {
        ApiError::setError(response,401,"Invalid password or user");
        LOG(INFO) << "GET LOGIN: \t\t No se recibio header Authorization";
        return;
    }

    std::string usr, pass;
    this->decodeAuth(usr_pass_b64, usr, pass);

    LOG(INFO) << "LOGIN REQUEST:\n"
              << "\t\tHeader Authorization: " << usr_pass_b64 << "\n"
              << "\t\tUser: " << usr << " Password: " << pass << std::endl;

    try {
        if (db->is_correct(usr, pass)) {  // si matchea la contrasena

            std::string token = sessionManager->add_session(usr, pass);;
            if (token == "") {
                ApiError::setError(response,500,"Internal server error");
            } else {
                response[STATUS] = SUCCES;
                response[DATA]["token"] = token;
            }
        } else {
            ApiError::setError(response,401,"Invalid password or user");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response,401,"Invalid password or user");
    }
    LOG(INFO) << "LOGIN RESPONSE:\n"
              << "\t\t" << response
              << std::endl;
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