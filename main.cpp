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

#define MSG_USAGE "<port> [-d -t]"
#define MSG_HELP_USAGE "<port>  ----> port number \n-d \t----> log debug \n-t \t----> testing empty db"


Json::Value generate_user(string &username);

std::string set_db_locations(string &db_accounts, string &db_users, string &db_sessions,
                      string &db_chats, string &db_images, bool &testing);

void parse_argv(int argc, char **pString, int &port, bool &debug, bool &testing);

volatile static bool running = true;

void handle_signal(int sig) {
    if (running) {
        cerr << "Exiting..." << endl;
        running = false;
    }
}

int main(int argc, char *argv[]) {
    int port = 8000;
    bool debug = false;
    bool testing = false;
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        std::cout << "Usage: " << argv[0] << MSG_USAGE << "\n"
                               << MSG_HELP_USAGE << std::endl;
        return 1;
    } else {
        parse_argv(argc, argv, port, debug, testing);
    }

    LOG(INFO) << " Port: " << port
              << "\n\t\t\t\t\t\t\t\tDebug: " << std::to_string(debug)
              << "\n\t\t\t\t\t\t\t\tTesting: " << std::to_string(testing);

    string db_accounts;
    string db_users;
    string db_sessions;
    string db_chats;
    string db_images;
    string db_dir = set_db_locations(db_accounts, db_users, db_sessions, db_chats,
                     db_images, testing);


    DatabaseManager db(db_accounts, db_users, db_sessions, db_chats, db_images);
    if (!db.openDBs())
        return -1;

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGKILL, handle_signal);

    SessionManager sessionManager(&db, ONE_HOUR);  //5 mins

    ProfileController pf(&db, &sessionManager);
    ChatController ch(&db, &sessionManager, "AIzaSyD91cge26COB1UtGA8IDsa4Jg7ZIHUEJME");

    ClientSharedServer ssClient(URLHEROKU);
    SharedServerController ssc(&ssClient, &sessionManager);

    JobifyServer server(8000);

    server.registerController(&pf);
    server.registerController(&ch);
    server.registerController(&ssc);

    server.start();
    LOG(INFO) << "Server Initiated";

    cout << "Server started, routes:" << endl;
    pf.dumpRoutes();
    ch.dumpRoutes();
    ssc.dumpRoutes();

    while (running) {
        sleep(1000);
    }

    server.stop();
    if (testing) {
        std::string delete_cmd ="rm -r "+db_dir;
        system(delete_cmd.c_str());
        LOG(INFO) << "Deleting DBs" << endl;
    }
    return 0;
}

void parse_argv(int argc, char **argv, int &port, bool &debug, bool &testing) {
    for (int i = 1; i < argc; i++) {
        if (isdigit(argv[i][0]) != 0)
            port = atoi(argv[i]);
        if (strcmp(argv[i], "-d") == 0)
            debug = true;
        if (strcmp(argv[i], "-t") == 0)
            testing = true;
    }
}

std::string set_db_locations(string &db_accounts, string &db_users, string &db_sessions,
                 string &db_chats, string &db_images, bool &testing) {
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    string db_dir(homedir);
    if(testing)
        db_dir += "/.Jobify_Appserver/testing/dbs";
    else
        db_dir += "/.Jobify_Appserver/dbs";

    string db_command = "mkdir -p ";
    db_command += db_dir;

    LOG(INFO) << "DATABASES LOCATION ----> " << db_dir;

    system(db_command.c_str());

    db_accounts = db_dir + "/accounts";
    db_users = db_dir + "/users";
    db_sessions = db_dir + "/sessions";
    db_chats = db_dir + "/chats";
    db_images = db_dir + "/images";

    return db_dir;
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
