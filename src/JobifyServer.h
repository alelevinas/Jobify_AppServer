//
// Created by ale on 15/09/16.
//

#ifndef JOBIFY_APPSERVER_SERVIDOR_H
#define JOBIFY_APPSERVER_SERVIDOR_H


#include <Server.h>

class JobifyServer : public Mongoose::Server {

public:
    JobifyServer(int port);

    virtual ~JobifyServer();
};


#endif //JOBIFY_APPSERVER_SERVIDOR_H
