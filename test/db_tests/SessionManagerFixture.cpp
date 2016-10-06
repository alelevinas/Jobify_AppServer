//
// Created by ale on 6/10/16.
//

#include <gtest/gtest.h>
#include <databases/UsersDB.h>
#include <SessionManager.h>
#include <exceptions/TokenDoesntExistException.h>

class SessionManagerFixture : public ::testing::Test {

protected:
    virtual void TearDown() {
        delete databaseManager;
        delete sessions_manager;
    }

    virtual void SetUp() {
        databaseManager = new DatabaseManager("testing_users","testing_sessions","testing_chats");
        sessions_manager = new SessionManager(databaseManager);
    }

public:
    SessionManagerFixture() : Test() {

    }

    virtual ~SessionManagerFixture() {
        system("rm -r testing_users");
        system("rm -r testing_sessions");
        system("rm -r testing_chats");
    }

    DatabaseManager* databaseManager;
    SessionManager *sessions_manager;

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
};


TEST_F(SessionManagerFixture, test_open_bds_are_ok) {
    databaseManager->openDBs();
    EXPECT_TRUE(true);
}

TEST_F(SessionManagerFixture, test_get_token_raises_exception) {
    EXPECT_TRUE(databaseManager->openDBs());

    string username = "lalala";

    EXPECT_THROW(sessions_manager->get_username(username),TokenDoesntExistException);
}

TEST_F(SessionManagerFixture, test_add_username_password) {
    EXPECT_TRUE(databaseManager->openDBs());

    string username = "alepox";
    string password = "not123456";

    string token = sessions_manager->add_session(username,password); //falla si salta una excepcion
    EXPECT_TRUE(true);
}

TEST_F(SessionManagerFixture, test_add_username_password_twice) {
    EXPECT_TRUE(databaseManager->openDBs());

    string username = "alepox";
    string password = "not123456";

    string token = sessions_manager->add_session(username,password); //falla si salta una excepcion

    string savedUsername = sessions_manager->get_username(token);

    EXPECT_EQ(username,savedUsername);
}


