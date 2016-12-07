//
// Created by nicolas on 29/11/16.
//

#ifndef JOBIFY_APPSERVER_CLIENTSHAREDSERVER_H
#define JOBIFY_APPSERVER_CLIENTSHAREDSERVER_H

#include <string>
#include <restclient-cpp/restclient.h>
#include <json/json.h>

#define URLLOCAL "http://localhost:8080/"
#define URLHEROKU "tallersharedserver.herokuapp.com/"
#define JOB_POSITIONS "job_positions"
#define SKILLS "skills"
#define CATEGORIES "categories"

/**
 * HTTP Client used to get data from the shared server instance.
 */
class ClientSharedServer {
private:
    std::string url; /// url of the shared server
public:
    ClientSharedServer(std::string url);

    std::string getJobPositions();

    /**
     * Gets the job Position names from the shared server.
     * @param names Json array to store the result
     * @return true if got result
     */
    bool getNamesJobPositions(Json::Value *names);

    /**skills names from the shared server.
     * @param names Json array to store the result
     * @return true if got result
     */
    bool getNamesSkills(Json::Value *names);

    /**
     * Gets the categories names from the shared server.
     * @param names Json array to store the result
     * @return true if got result
     */
    bool getNamesCategories(Json::Value *names);
};



#endif //JOBIFY_APPSERVER_CLIENTSHAREDSERVER_H
