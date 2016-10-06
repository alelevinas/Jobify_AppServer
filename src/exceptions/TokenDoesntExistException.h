//
// Created by ale on 6/10/16.
//



#include <exception>

class TokenDoesntExistException: public std::exception {

    virtual const char* what() const throw() {
        return "There is NO token like that in the database";
    }
};