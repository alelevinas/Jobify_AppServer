//
// Created by ale on 12/10/16.
//

#ifndef JOBIFY_APPSERVER_INVALIDUSERNAMEPASSWORDEXCEPTION_H
#define JOBIFY_APPSERVER_INVALIDUSERNAMEPASSWORDEXCEPTION_H



#include <exception>

class InvalidUsernamePasswordException: public std::exception {

    virtual const char* what() const throw() {
        return "There is NO username with that password in the database";
    }
};

#endif //JOBIFY_APPSERVER_INVALIDUSERNAMEPASSWORDEXCEPTION_H
