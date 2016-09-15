//
// Created by ale on 15/09/16.
//

#include "JobifyServer.h"

using namespace Mongoose;

JobifyServer::JobifyServer(int port) : Server(port) {
}

JobifyServer::~JobifyServer() {
    //Para el servidor en su destructor
}
