//
// Created by ale on 14/09/16.
//

#ifndef JOBIFY_APPSERVER_CHATSDB_H
#define JOBIFY_APPSERVER_CHATSDB_H


#include <string>
#include <leveldb/db.h>
#include "DB.h"
#include "UsersDB.h"
/**
 * DB that stores the chat messages data from users conversations.
 */
class ChatsDB: public DB{

public:
    ChatsDB(std::string& dbName);

    virtual ~ChatsDB();

    /**
     * Stores a message in the DB. The keys will be  "@param username_from + _ + @param user_to" and
     * "@param user_to + _ + @param username_from"
     * @param username_from The username that sends the message
     * @param name_from the name of @param username_from user
     * @param user_to the destination user's username
     * @param message the message body
     * @return true if no problem
     */
    bool add_msg(std::string username_from, std::string name_from, std::string user_to, std::string message);

    /**
     * Assigns an "id" to the messages.
     * @return True if no database errors
     */
    bool updateMessages(std::string, Json::Value);


    /**
     * Gets the conversation between username and username2
     * @param username
     * @param username2
     * @param conversation to store the result
     * @return true if no DB errors
     */
    bool get_conv(std::string username, std::string username2, Json::Value *conversation);

    /**
     * Gets every conversation in the DB
     * @param username
     * @param conversations
     * @param pDB
     * @return
     */
    bool get_convs(std::string username, Json::Value *conversations, UsersDB *pDB);

    /**
     * Deletes the conversation between username and username2
     * @param username
     * @param username2
     * @return
     */
    bool delete_conv(std::string username, std::string username2);

    /**
     * Deletes the message with the @param idMensaje in the conversation between @param username1 and @param username2
     * @param username1
     * @param username2
     * @param idMensaje
     * @return True if the message could be deleted
     */
    bool delete_message(std::string username1, std::string username2, std::string idMensaje);
};


#endif //JOBIFY_APPSERVER_CHATSDB_H
