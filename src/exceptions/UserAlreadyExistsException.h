//
// Created by ale on 2/10/16.
//

#ifndef JOBIFY_APPSERVER_USERALREADYEXISTSEXCEPTION_H
#define JOBIFY_APPSERVER_USERALREADYEXISTSEXCEPTION_H

#include <exception>

class UserAlreadyExistsException: public std::exception {

virtual const char* what() const throw() {
    return "There is ALREADY an user with that username in the database";
}
};

#endif //JOBIFY_APPSERVER_USERALREADYEXISTSEXCEPTION_H
