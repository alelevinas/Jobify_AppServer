#include <iostream>
#include <signal.h>
#include <DatabaseManager.h>
#include <ProfileController.h>
#include <JobifyServer.h>
#include <zconf.h>
#include <log/easylogging++.h>
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
    //Foo f;

    //f.hola();

    signal(SIGTERM, handle_signal);

    DatabaseManager db("accounts","userss", "sessions", "chats");
    if (!db.openDBs())
        return -1;

//    std::string username("pepe");
//
//    Json::Value pepe = generate_user(username);
//    db.add_user("pepe",pepe);

    SessionManager sessionManager(&db,ONE_HOUR);  //5 mins

    ProfileController pf(&db, &sessionManager);
    ChatController ch(&db, &sessionManager);

    JobifyServer server(8000);

    server.registerController(&pf);

    server.start();
    //server.printStats();
    LOG(INFO) << "Iniciando Servidor";

    cout << "Server started, routes:" << endl;
    pf.dumpRoutes();

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
