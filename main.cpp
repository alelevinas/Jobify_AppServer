#include <iostream>
#include <signal.h>
#include <DatabaseManager.h>
#include <ProfileController.h>
#include <JobifyServer.h>
#include <zconf.h>
#include <pwd.h>
#include <log/easylogging++.h>
#include <ClientSharedServer.h>
#include <SharedServerController.h>
#include "ChatController.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE


Json::Value generate_user(string &username);

volatile static bool running = true;

void handle_signal(int sig)
{
    if (running) {
        cerr << "Exiting..." << endl;
        running = false;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    std::string db_dir(homedir);
    db_dir+="/.Jobify_Appserver/dbs";

    std::string db_command = "mkdir -p ";
    db_command+=db_dir;

    LOG(INFO) << "DATABASES LOCATION ----> " << db_dir;

    system(db_command.c_str());


    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    std::string db_accounts = db_dir+"/accounts";
    std::string db_users = db_dir+"/users";
    std::string db_sessions = db_dir+"/sessions";
    std::string db_chats = db_dir+"/chats";
    std::string db_images = db_dir+"/images";

    DatabaseManager db(db_accounts,db_users, db_sessions, db_chats, db_images);
    if (!db.openDBs())
        return -1;

    SessionManager sessionManager(&db,ONE_HOUR);  //5 mins

    ProfileController pf(&db, &sessionManager);
    ChatController ch(&db, &sessionManager, "AAAA425dth8:APA91bH81cAehi-kJ1JOGVHNhmn_rx7Meqm5QL1a6JUq89LlGAtBx1-5QIYOIwGE9XyHiOJgEBP5RGgBnowcvKV8uBcyAiLE6z8YBLvGCrKJfpSYt0dBYS972A3RLsYWnBbx8G9YBV2LEJYLCMwa8Ipca_1kme8TRw");

    ClientSharedServer ssClient(URLHEROKU);
    SharedServerController ssc(&ssClient, &sessionManager);

    JobifyServer server(8000);

    server.registerController(&pf);
    server.registerController(&ch);
    server.registerController(&ssc);

    server.start();
    LOG(INFO) << "Iniciando Servidor";

    cout << "Server started, routes:" << endl;
    pf.dumpRoutes();
    ch.dumpRoutes();
    ssc.dumpRoutes();

    while (running) {
        sleep(1000);
    }

    server.stop();
    return 0;
}

Json::Value generate_user(string &username) {
    Json::Value user;
    user["username"] = username;
    user["name"] = "Alejandro Pablo Levinas";
    user["gender"] = "male";
    user["email"] = "lolo@gmail.com";
    user["dob"] = "1993-08-19";
    user["city"] = "CABA";
    user["nationality"] = "argentino";
    user["profile"] = "Soy un estudiante de ingenieria en informatica que "
            "se propone............blabllbla...........";

    Json::Value skills(Json::arrayValue);
    skills.append(Json::Value("C"));
    skills.append(Json::Value("C++"));
    skills.append(Json::Value("GoogleTest"));
    user["skills"] = skills;

    Json::Value exp(Json::arrayValue);
    Json::Value job1;
    job1["years"] = "2006-2009";
    job1["company"] = "NASA";
    job1["position"] = "Desarrollador";
    job1["description"] = "Desarrollador en lenguaje R para analizar......";

    Json::Value job2;
    job2["years"] = "2010-actualidad";
    job2["company"] = "UBA";
    job2["position"] = "Docente";
    job2["description"] = "Docente de la materia Taller 2";

    exp.append(job1);
    exp.append(job2);
    user["previous_exp"] = exp;

    Json::Value cont(Json::arrayValue);
    user["contacts"] = cont;
    user["recomendations"] = cont;
    user["chats"] = cont;

    return user;
}
