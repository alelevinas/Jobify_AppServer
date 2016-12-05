//
// Created by ale on 30/11/16.
//

#include <json/json.h>
#include <log/easylogging++.h>
#include <restclient-cpp/restclient.h>
#include "restclient-cpp/connection.h"


#include "NotificationSender.h"

NotificationSender::NotificationSender(const std::string &server_key) : server_key(server_key) {}

NotificationSender::~NotificationSender() {

}


/*
 * ENVIAR ESTO
 * curl --header "Authorization: key=<Server Key>" --header "Content-Type: application/json"\
 *              https://android.googleapis.com/gcm/send\
 *              -d '{"notification":{"title":"Hi","body":"Hello from the Cloud"},"data":{"score":"lots"},"to":"<Registration Token>"}'
 *
 */
void NotificationSender::send_notification(std::string from, std::string dest_reg_token, std::string message) {
    Json::Value notification;

    create_notification(notification,from,dest_reg_token,message);

    // initialize RestClient
    RestClient::init();

// get a connection object
    RestClient::Connection* conn = new RestClient::Connection("https://android.googleapis.com/gcm/send");
    // set headers
    RestClient::HeaderFields headers;
    headers["Authorization"] = "key="+server_key;
    headers["Content-Type"] = "application/json";
    conn->SetHeaders(headers);


    LOG(INFO) << "Envio a Firebase: " << notification; //r.body r.code etc
    RestClient::Response r = conn->post("/post", notification.asString());

    // deinit RestClient. After calling this you have to call RestClient::init()
// again before you can use it
    RestClient::disable();


    LOG(INFO) << "Respuesta de Firebase: " << r.body; //r.body r.code etc
}


/*
 * '{"notification":{"title":"Hi","body":"Hello from the Cloud"},"to":"<Registration Token>"
 */
void NotificationSender::create_notification(Json::Value &notification, std::string &from, std::string &dest_reg_token,
                                             std::string &message) {
    Json::Value user_viewable;
    user_viewable["title"] = "New message from "+from;
    user_viewable["body"] = message;
    notification["notification"] = user_viewable;
    notification["to"] = dest_reg_token;
}
