//
// Created by nicolas on 29/11/16.
//

#include "ClientSharedServer.h"
#include <sstream>
#include <iostream>

ClientSharedServer::ClientSharedServer(std::string url, int port) {
    std::stringstream ss;
    ss << url << ":" << port;
    this->url = ss.str();
}

std::string ClientSharedServer::getJobPositions() {
    std::string sUrlJobPos = this->url + "/" + JOB_POSITIONS;
    RestClient::Response r;
    r = RestClient::get(sUrlJobPos);
    return r.body;
}
