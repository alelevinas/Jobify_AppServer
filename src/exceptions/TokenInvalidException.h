//
// Created by ale on 6/10/16.
//

#ifndef JOBIFY_APPSERVER_TOKENINVALIDEXCEPTION_H
#define JOBIFY_APPSERVER_TOKENINVALIDEXCEPTION_H

#include <exception>

class TokenInvalidException: public std::exception {

    virtual const char* what() const throw() {
        return "There is NO token like that in the database";
    }
};

#endif //JOBIFY_APPSERVER_TOKENINVALIDEXCEPTION_H