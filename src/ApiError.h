//
// Created by ale on 11/11/16.
//

#ifndef JOBIFY_APPSERVER_APIERROR_H
#define JOBIFY_APPSERVER_APIERROR_H


#include <JsonResponse.h>

#define STATUS "status"
#define SUCCES "succes"
#define OK "OK"
#define ERROR "error"
#define DATA "data"

class ApiError {

public:
    static void setError(Mongoose::JsonResponse &response, int code, const char* message) {
        response.setCode(code);
        response[STATUS] = ERROR;
        response[DATA] = "";
        response[ERROR]["code"] = code;
        response[ERROR]["message"] = message;
    }

};


#endif //JOBIFY_APPSERVER_APIERROR_H
