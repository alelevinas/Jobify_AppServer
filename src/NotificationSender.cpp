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
 *              https://fcm.googleapis.com/fcm/send\
 *              -d '{"notification":{"title":"Hi","body":"Hello from the Cloud"},"data":{"score":"lots"},"to":"<Registration Token>"}'
 *
 */
void NotificationSender::send_notification(std::string from_username, std::string dest_reg_token, std::string message,
                                           std::string from_name) {
    Json::Value notification;

    create_notification(notification, from_username, dest_reg_token, message, from_name);

    // initialize RestClient
    RestClient::init();

// get a connection object
    RestClient::Connection* conn = new RestClient::Connection("https://fcm.googleapis.com/fcm/send");
    // set headers
    RestClient::HeaderFields headers;
    headers["Authorization"] = "key="+server_key;
    headers["Content-Type"] = "application/json";
    conn->SetHeaders(headers);


    LOG(INFO) << "Envio a Firebase: " << notification; //r.body r.code etc

    std::stringstream ss;
    ss << notification;

    LOG(DEBUG) << "Stringstremeado " << ss.str();

    RestClient::Response r = conn->post("", ss.str());

    LOG(INFO) << "Respuesta de Firebase: " << r.body; //r.body r.code etc

    // deinit RestClient. After calling this you have to call RestClient::init()
// again before you can use it
    RestClient::disable();



}


/*
 * '{"notification":{"title":"Hi","body":"Hello from the Cloud"},"to":"<Registration Token>"
 */
void NotificationSender::create_notification(Json::Value &notification, std::string &from_username,
                                             std::string &dest_reg_token, std::string &message, std::string &from_name) {
    Json::Value user_viewable;
    user_viewable["title"] = from_name;
    user_viewable["body"] = message;
    user_viewable["click_action"] = "OPEN_ACTIVITY_1";
    user_viewable["color"] = "#3b5998";
    user_viewable["icon"] = "ic_discuss";
    notification["notification"] = user_viewable;
    Json::Value data;
    data["username"] = from_username;
    data["message"] = message;
    data["name"] = "ALEPOX";
    notification["data"] = data;
    notification["to"] = dest_reg_token;
}
