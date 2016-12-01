//
// Created by ale on 30/11/16.
//

#ifndef JOBIFY_APPSERVER_NOTIFICATIONSENDER_H
#define JOBIFY_APPSERVER_NOTIFICATIONSENDER_H

#include <string>

class NotificationSender {

    std::string server_key; //AIzaSyD91cge26COB1UtGA8IDsa4Jg7ZIHUEJME al 23/11/2016

public:
    NotificationSender(const std::string &server_key);

    void send_notification(std::string from, std::string dest_reg_token, std::string message);

    virtual ~NotificationSender();


    void
    create_notification(Json::Value &notification, std::string &from, std::string &dest_reg_token, std::string &message);
};


#endif //JOBIFY_APPSERVER_NOTIFICATIONSENDER_H
