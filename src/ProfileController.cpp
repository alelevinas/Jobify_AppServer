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
//    cerr << "\ntoken recibido " << token;

    std::string username;
    try {
        username = sessionManager->get_username(token);

        LOG(INFO) << "USER GET REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username
                  << std::endl;

//        cerr << " es del usuario: " << username;

        Json::Value user = db->get_user(username);

        user["username"] = username;

        if (user["username"] != username) {
            ApiError::setError(response, 500, "Internal server error");
        } else {
            response[STATUS] = SUCCES;
            response[DATA] = user;
        }

    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "token invalido");
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 500, "Internal server error");
    }
    LOG(INFO) << "USER GET RESPONSE:\n"
              << "\t\tUser: " << username << "\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

void ProfileController::getUsersRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {

//    cerr << "\ntoken recibido " << token;
    //std::string paramQ = request.get("q","hola");
    //std::string body = request.getData();
    try {
        //std::string token = request.getHeaderKeyValue("Token");

        //cerr << " es del usuario: " << username;

        //std::string username = sessionManager->get_username(token);

        LOG(INFO) << "USERS GET REQUEST:\n"
                  //        << "\t\tHeader Token: " << token << "\n"
                  //      << "\t\tUser: " << username
                  << std::endl;

        response[STATUS] = SUCCES;

        Json::Value users;
        if (!db->get_users(users)) {
            ApiError::setError(response, 500, "Internal server error");
        }
        response[DATA] = users["users"]; //le agrego esto a pedido


    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "token invalido");
    }
    LOG(INFO) << "USERS RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

//signup
void ProfileController::postUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string usr_pass_b64 = request.getHeaderKeyValue("Authorization");
    std::string usr, pass;
    this->decodeAuth(usr_pass_b64, usr, pass);

    std::string firebase_reg_token = request.getHeaderKeyValue("FirebaseToken");


    std::string json_user = request.getData(); //el body

    LOG(INFO) << "SIGNUP REQUEST:\n"
              << "\t\tHeader Authorization: " << usr_pass_b64 << "\n"
              << "\t\tUser: " << usr << " Password: " << pass << "\n"
              << "\t\tData: " << json_user
              << std::endl;

    bool ok = true;
    if (usr_pass_b64.empty()) {
        ok = false;
        ApiError::setError(response, 400, "Wrong CREDENTIALS");
    }

    Json::Reader reader;
    Json::Value user;

    bool parsingSuccessful = reader.parse(json_user, user);
    if (ok && !parsingSuccessful) {
        ok = false;
        ApiError::setError(response, 410, "Wrong JSON");  // TODO agregar a la API documentation
    }
    user["username"] = usr;
    user["coordenates"] = "";
    user["firebase_token"] = firebase_reg_token;
    if (user["chats"].empty())
        user["chats"] = Json::Value(Json::arrayValue);
    if (user["contacts"].empty())
        user["contacts"] = Json::Value(Json::arrayValue);
    if (user["recommended_by"].empty())
        user["recommended_by"] = Json::Value(Json::arrayValue);
    try {
        if (ok && !db->add_account(usr, pass)) {
            ok = false;
            ApiError::setError(response, 500, "Internal server error");
        }
        if (ok && !db->add_user(usr, user)) {
            ok = false;
            //TODO remove_account que nisiquiera existe
            ApiError::setError(response, 500, "Internal server error");
        }
        if (ok) {
            std::string token = sessionManager->add_session(usr, pass);;
            if (token == "") {
                //TODO remove account and user
                ApiError::setError(response, 500, "Internal server error");
            } else {
                response[STATUS] = SUCCES;
                response[DATA]["token"] = token;
            }
        }
    } catch (KeyAlreadyExistsException &e) {
        ApiError::setError(response, 409, "Email already in use");
    }
    LOG(INFO) << "SIGNUP RESPONSE:\n"
              << "\t\t" << this->logResponse(response)
              << std::endl;
}

void ProfileController::updateUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
//    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);


//        cerr << " es del usuario: " << username;

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
            ApiError::setError(response, 410, "Wrong JSON");  // TODO agregar a la API documentation
            ok = false;
        }
        if (ok && db->edit_user(username, edited_user)) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        } else {
            ApiError::setError(response, 500, "Internal server error");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 500, "Internal server error");
    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "token invalido");
    }
    LOG(INFO) << "UPDATE USER RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

void ProfileController::deleteUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
//    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

        LOG(INFO) << "DELETE USER REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username;

//        cerr << " es del usuario: " << username;

        bool ok = true;
        if (!db->delete_user(username)) {
            ApiError::setError(response, 500, "Internal server error");
            ok = false;
        }
        if (!db->delete_session(token)) {
            // volver a poner el usuario?
            ApiError::setError(response, 500, "Internal server error");
            ok = false;
        } //todo delete account
        if (ok) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 500, "Internal server error");
    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "token invalido");
    }
    LOG(INFO) << "DELETE USER RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

void ProfileController::getLogin(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string usr_pass_b64 = request.getHeaderKeyValue("Authorization");
    if (usr_pass_b64.empty()) {
        ApiError::setError(response, 401, "Invalid password or user");
        LOG(INFO) << "GET LOGIN: \t\t No se recibio header Authorization";
        return;
    }
    std::string firebase_reg_token = request.getHeaderKeyValue("FirebaseToken");

    std::string usr, pass;
    this->decodeAuth(usr_pass_b64, usr, pass);

    LOG(INFO) << "LOGIN REQUEST:\n"
              << "\t\tHeader Authorization: " << usr_pass_b64 << "\n"
              << "\t\tHeader Firebase Token: " << firebase_reg_token << "\n"
              << "\t\tUser: " << usr << " Password: " << pass << std::endl;

    try {
        if (db->is_correct(usr, pass)) {  // si matchea la contrasena

            // actualizo el token de notificaciones
            Json::Value user = db->get_user(usr);
            user["firebase_token"] = firebase_reg_token;
            db->edit_user(usr,user);

            std::string token = sessionManager->add_session(usr, pass);
            if (token == "") {
                ApiError::setError(response, 500, "Internal server error");
            } else {
                response[STATUS] = SUCCES;
                response[DATA]["token"] = token;
            }
        } else {
            ApiError::setError(response, 401, "Invalid password or user");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 401, "Invalid password or user");
    } catch (std::exception &e) {
        LOG(DEBUG) << e.what();
    }
    LOG(INFO) << "LOGIN RESPONSE:\n"
              << "\t\t" << this->logResponse(response)
              << std::endl;
}

void ProfileController::postRecommend(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
//    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

//        cerr << " es del usuario: " << username;

        std::string recommendTo = request.getData(); //el body

        LOG(INFO) << "RECOMMEND USER REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << "\n"
                  << "\t\tData: " << recommendTo
                  << std::endl;

        db->get_user(username); //solo para ver si salta la exception

        Json::Reader reader;
        Json::Value userToRecomend;
        bool parsingSuccessful = reader.parse(recommendTo, userToRecomend);
        if (!parsingSuccessful) {
            ApiError::setError(response, 410, "Wrong JSON");  // TODO agregar a la API documentation
            LOG(INFO) << "RECOMMEND USER RESPONSE:\n"
                      << "\t\tResponse: " << this->logResponse(response)
                      << std::endl;
            return;
        }


        if (db->recommend_user(username, userToRecomend["username"].asString())) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        } else {
            ApiError::setError(response, 500, "Internal server error");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 500, "Internal server error");
    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "invalid token");
    }
    LOG(INFO) << "RECOMMEND USER RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

void ProfileController::deleteDeRecommend(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
//    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

//        cerr << " es del usuario: " << username;

        std::string deRecommendTo = request.getData(); //el body

        LOG(INFO) << "DERECOMMEND USER REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << "\n"
                  << "\t\tData: " << deRecommendTo
                  << std::endl;

        db->get_user(username); //solo para ver si salta la exception

        Json::Reader reader;
        Json::Value userToRecomend;
        bool parsingSuccessful = reader.parse(deRecommendTo, userToRecomend);
        if (!parsingSuccessful) {
            ApiError::setError(response, 410, "Wrong JSON");  // TODO agregar a la API documentation
            LOG(INFO) << "DERECOMMEND USER RESPONSE:\n"
                      << "\t\tResponse: " << this->logResponse(response)
                      << std::endl;
        }
        if (db->deRecommend_user(username, userToRecomend["username"].asString())) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        } else {
            ApiError::setError(response, 420, "User was not recommended before");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 500, "Internal server error");
    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "invalid token");
    }
    LOG(INFO) << "DERECOMMEND USER RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

void ProfileController::getUserContacts(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");

    try {
        std::string username = sessionManager->get_username(token);


        LOG(INFO) << "GET CONTACTS USERS REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << "\n"
                  << std::endl;

        //Json::Value user = db->get_user(username);

        response[STATUS] = SUCCES;
        Json::Value contacts(Json::arrayValue);

        if (!db->get_user_contacts(username, contacts))
            ApiError::setError(response, 500, "Internal server error");
        else
            response[DATA] = contacts;

    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "invalid token");
    }
    LOG(INFO) << "GET CONTACTS USERS RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;

}

void ProfileController::postAddContact(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
//    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

//        cerr << " es del usuario: " << username;

        std::string contactToAdd = request.getData(); //el body

        LOG(INFO) << "ADD CONTACT USER REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << "\n"
                  << "\t\tData: " << contactToAdd
                  << std::endl;

        db->get_user(username); //solo para ver si salta la exception

        Json::Reader reader;
        Json::Value userToAdd;
        bool parsingSuccessful = reader.parse(contactToAdd, userToAdd);
        if (!parsingSuccessful) {
            ApiError::setError(response, 410, "Wrong JSON");  // TODO agregar a la API documentation
            LOG(INFO) << "ADD CONTACT USER RESPONSE:\n"
                      << "\t\tResponse: " << this->logResponse(response)
                      << std::endl;
            return;
        }


        if (db->addContact(username, userToAdd["username"].asString())) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        } else {
            ApiError::setError(response, 500, "Internal server error");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 500, "Internal server error");
    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "invalid token");
    }
    LOG(INFO) << "ADD CONTACT USER RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

void ProfileController::deleteRemoveContact(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");
//    cerr << "\ntoken recibido " << token;

    try {
        std::string username = sessionManager->get_username(token);

//        cerr << " es del usuario: " << username;

        std::string contactToRemove = request.getData(); //el body

        LOG(INFO) << "REMOVE CONTACT USER REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << "\n"
                  << "\t\tData: " << contactToRemove
                  << std::endl;

        db->get_user(username); //solo para ver si salta la exception

        Json::Reader reader;
        Json::Value userToRemove;
        bool parsingSuccessful = reader.parse(contactToRemove, userToRemove);
        if (!parsingSuccessful) {
            ApiError::setError(response, 410, "Wrong JSON");  // TODO agregar a la API documentation
            LOG(INFO) << "DERECOMMEND USER RESPONSE:\n"
                      << "\t\tResponse: " << this->logResponse(response)
                      << std::endl;
        }
        if (db->removeContact(username, userToRemove["username"].asString())) {
            response[STATUS] = SUCCES;
            response[DATA] = "ok";
        } else {
            ApiError::setError(response, 420, "User was not a contact before");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 500, "Internal server error");
    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "invalid token");
    }
    LOG(INFO) << "REMOVE CONTACT USER RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}

void ProfileController::getFilteredUsers(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string token = request.getHeaderKeyValue("Token");

    try {
        std::string username = sessionManager->get_username(token);

        std::string qSort = request.get("sort", "");
        std::string qFilter = request.get("filter", "");
        std::string qJob = request.get("job_position", "");
        std::string qSkill = request.get("skill", "");
        std::string qDistance = request.get("distance", ""); //solo los que estan a distancia menor a...

        LOG(INFO) << "GET SEARCH USERS REQUEST:\n"
                  << "\t\tHeader Token: " << token << "\n"
                  << "\t\tUser: " << username << "\n"
                  << "\t\tQuery: "
                  << "\t\t\t\t sorting " << qSort
                  << "\t\t\t\t filter_by " << qFilter
                  << "\t\t\t\t job_position " << qJob
                  << "\t\t\t\t skill " << qSkill
                  << std::endl;

        int nFilter, nDistance;
        if (!qFilter.empty())
            nFilter = stoi(qFilter);
        else
            nFilter = 10;
        if (!qDistance.empty())
            nDistance = stoi(qDistance);
        else
            nDistance = 10000;
        /*
         * REALIZAR CONSULTAS CON LAS BASES DE DATOS
         *
         * url: /users/search?sort=qSort&filter=qFilter&job_position=qJob&skill=qSkill
         *
         * example: /users/search?sort=recommended&filter=10&job_position=developer&skill=java&distance=100
         *
         */
        Json::Value user = db->get_user(username);

        string caller_coordenates = user["coordenates"].asString();

        response[STATUS] = SUCCES;

        Json::Value users(Json::arrayValue);

        // Las caller_coordenates deben tener el formato "xxx:yyy"
        if (!db->get_users_by(qSort, nFilter, qJob, qSkill, users, nDistance,
                              caller_coordenates))
            ApiError::setError(response, 500, "Internal server error");
        else
            response[DATA] = users;

    } catch (TokenInvalidException &e) {
        ApiError::setError(response, 501, "invalid token");
    }
    LOG(INFO) << "GET SEARCH USERS RESPONSE:\n"
              << "\t\tResponse: " << this->logResponse(response)
              << std::endl;
}


void ProfileController::deleteAllBDs(Mongoose::Request &request, Mongoose::JsonResponse &response) {
    std::string usr_pass_b64 = request.getHeaderKeyValue("Authorization");
    if (usr_pass_b64.empty()) {
        ApiError::setError(response, 401, "Invalid password or user");
        LOG(INFO) << "DELETE BDs: \t\t No se recibio header Authorization";
        return;
    }

    std::string usr, pass;
    this->decodeAuth(usr_pass_b64, usr, pass);

    LOG(INFO) << "DELETE BDs REQUEST:\n"
              << "\t\tHeader Authorization: " << usr_pass_b64 << "\n"
              << "\t\tUser: " << usr << " Password: " << pass << std::endl;

    try {
        if (usr == "admin" and pass == "admin") {  // si matchea la contrasena
            if (!db->deleteDBs()) {
                ApiError::setError(response, 500, "Internal server error");
            } else {
                response[STATUS] = SUCCES;
                response[DATA] = "ok";
            }
        } else {
            ApiError::setError(response, 401, "Invalid password or user");
        }
    } catch (KeyDoesntExistException &e) {
        ApiError::setError(response, 401, "Invalid password or user");
    }
    LOG(INFO) << "DELETE BDs RESPONSE:\n"
              << "\t\t" << this->logResponse(response)
              << std::endl;
}

Json::Value ProfileController::logResponse(Mongoose::JsonResponse &response) {
    Json::Value copy = response;
    Json::Value &data = copy["data"];
    if (data.isArray()) {
        for (Json::Value &user : data) {
            if (user.isMember("picture"))
                user["picture"] = "";
        }
    } else if (data.isObject() and data.isMember("picture")) {
        data["picture"] = "";
    } else {
        return response;
    }

    return copy;
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

    addRouteResponse("POST", "/users/recommend", ProfileController, postRecommend, JsonResponse);
    addRouteResponse("DELETE", "/users/recommend", ProfileController, deleteDeRecommend, JsonResponse);
    addRouteResponse("POST", "/users/contacts", ProfileController, postAddContact, JsonResponse);
    addRouteResponse("DELETE", "/users/contacts", ProfileController, deleteRemoveContact, JsonResponse);

    addRouteResponse("GET", "/users/contacts", ProfileController, getUserContacts, JsonResponse);

    addRouteResponse("GET", "/users/search", ProfileController, getFilteredUsers, JsonResponse);

    addRouteResponse("DELETE", "/bds/delete", ProfileController, deleteAllBDs, JsonResponse);
}
