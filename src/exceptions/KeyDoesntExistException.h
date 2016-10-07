//
// Created by ale on 2/10/16.
//

#ifndef JOBIFY_APPSERVER_USERDOESNTEXISTEXCEPTION_H
#define JOBIFY_APPSERVER_USERDOESNTEXISTEXCEPTION_H

#include <iostream>
#include <exception>

class KeyDoesntExistException: public std::exception {

  virtual const char* what() const throw() {
    return "There is NO key with that name in the database";
  }
};


#endif //JOBIFY_APPSERVER_USERDOESNTEXISTEXCEPTION_H
