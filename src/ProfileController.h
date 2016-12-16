//
// Created by ale on 15/09/16.
//

#ifndef JOBIFY_APPSERVER_PROFILECONTROLLER_H
#define JOBIFY_APPSERVER_PROFILECONTROLLER_H


#include <JsonController.h>
#include "DatabaseManager.h"
#include "SessionManager.h"


/**
 * Controller that manages the user related api endpoints. Every method recieves a
 * Moongose::Request with the request data and a Mongoose::JsonResponse
 * to store the response json.
 */
class ProfileController : public Mongoose::JsonController{

    DatabaseManager* db; /// the DB
    SessionManager* sessionManager; /// The Session Manager instance that controls active users
public:
    ProfileController(DatabaseManager *db, SessionManager *sessionManager);

    void setup();

    /**
     * Gets the user specified in the query "username" and sets de response
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Gets the user specified in the request body and saves it in the DB. Then it sets de response
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void postUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Updates the user specified in the request body and saves it in the DB. Then it sets de response
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void updateUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Deletes from de DB the user specified in the decrypted header Token
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void deleteUserRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Gets all the users. Then it sets de response
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getUsersRequest(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Login endpoint. Checks if the username:password in b64 matches any on the DB and grants acces throug a token
     * or declines it
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getLogin(Mongoose::Request &request, Mongoose::JsonResponse &response);


    /**
     * Recommends the user specified in the body.
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void postRecommend(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Derecommends the user specified in the body.
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void deleteDeRecommend(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Gets the user's contacts.
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getUserContacts(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Adds the contact specified in the body
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void postAddContact(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Deletes the contact specified in the body
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void deleteRemoveContact(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Gets the users corresponding to the filter queries
     * "sort", "filter", "job_position", "skill" and "distance"
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void getFilteredUsers(Mongoose::Request &request, Mongoose::JsonResponse &response);

    /**
     * Deletes all BDs. Mainly for testing purposes. It demands Basic Authorization with
     * a special password
     * @param request The request data
     * @param response The JsonResponse to send
     */
    void deleteAllBDs(Mongoose::Request &request, Mongoose::JsonResponse &response);

private:

    /**
     * Decodes username:password in b64 and stores them in usr and pass
     * @param b64_auth the enconded string
     * @param usr the username decoded result
     * @param pass the password decoded result
     */
    void decodeAuth(std::string &b64_auth, std::string &usr, std::string &pass);

    /**
     * Internal function so the b64 image does not display in the log.
     * @param response the response to filter
     * @return a copy of the filtered response
     */
    Json::Value logResponse(Mongoose::JsonResponse& response);
};


#endif //JOBIFY_APPSERVER_PROFILECONTROLLER_H
