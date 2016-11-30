//
// Created by nicolas on 29/11/16.
//

#ifndef JOBIFY_APPSERVER_CLIENTSHAREDSERVER_H
#define JOBIFY_APPSERVER_CLIENTSHAREDSERVER_H

//sudo apt-get install autoconf
//sudo apt-get install libtool
//cd restclient-cpp
// ./autogen.sh
// ./configure
// sudo make install

#include <string>
#include <restclient-cpp/restclient.h>

#define URL "http://localhost"
#define PORT 8080
#define JOB_POSITIONS "job_positions"


class ClientSharedServer {
private:
    std::string url;
public:
    ClientSharedServer(std::string url, int port);
    std::string getJobPositions();
};



#endif //JOBIFY_APPSERVER_CLIENTSHAREDSERVER_H
