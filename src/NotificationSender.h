//
// Created by ale on 30/11/16.
//

#ifndef JOBIFY_APPSERVER_NOTIFICATIONSENDER_H
#define JOBIFY_APPSERVER_NOTIFICATIONSENDER_H

#include <string>

#define FIREBASE_SERVER_KEY "AIzaSyD91cge26COB1UtGA8IDsa4Jg7ZIHUEJME"

/**
 * Class made for sending notifications throug Google Firebase tokens
 */
class NotificationSender {

    std::string server_key; ///Firebase server_key
    //AIzaSyD91cge26COB1UtGA8IDsa4Jg7ZIHUEJME al 23/11/2016

public:
    NotificationSender(const std::string &server_key);

    /**
     * Send an HTTP post to firebase endpoint with the notification data.
     * @param from_username The sender of the message
     * @param dest_reg_token The firebase token of the destinatary devise
     * @param message The body of the message
     */
    void send_notification(std::string from_username, std::string dest_reg_token, std::string message,
                           std::string from_name);

    virtual ~NotificationSender();

private:
    /**
     * Creates the body of the HTTP post for notification
     * @param notification reference to store the result
     * @param from_username The sender of the message
     * @param dest_reg_token The firebase token of the destinatary devise
     * @param message The body of the message
     */
    void create_notification(Json::Value &notification, std::string &from_username,
                             std::string &dest_reg_token, std::string &message, std::string &from_name);
};


#endif //JOBIFY_APPSERVER_NOTIFICATIONSENDER_H
