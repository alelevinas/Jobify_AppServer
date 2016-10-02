//
// Created by ale on 11/09/16.
//

#include <gtest/gtest.h>
#include <UsersDB.h>
#include <exceptions/UserDoesntExistException.h>
#include <exceptions/UserAlreadyExistsException.h>

class UsersDBFixture : public ::testing::Test {

protected:
    virtual void TearDown() {
        delete db;
    }

    virtual void SetUp() {
        db = new UsersDB(db_name);
    }

public:
    UsersDBFixture() : Test() {

    }

    virtual ~UsersDBFixture() {
        system("rm -r testing");
    }

    UsersDB *db;
    string db_name = "testing";

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

TEST_F(UsersDBFixture, test_open_bd_is_ok){
    EXPECT_TRUE(db->openDB());
}

TEST_F(UsersDBFixture, test_add_user_alepox_in_empty_bd) {
    if (!db->openDB())
        EXPECT_TRUE(false);

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));
}

TEST_F(UsersDBFixture, test_add_user_alepox_twice_in_empty_bd) {
    if (!db->openDB())
        EXPECT_TRUE(false);

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));

    EXPECT_THROW(db->add_user(username, user), UserAlreadyExistsException);
}

TEST_F(UsersDBFixture, test_get_user_alepox_in_populated_bd){
    if (!db->openDB())
        EXPECT_TRUE(false);

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));
    Json::Value userFromBD = db->get_user(username);

    //std::cout << userFromBD << std::endl;

    EXPECT_EQ(userFromBD["username"],Json::Value(username));
}


TEST_F(UsersDBFixture, test_delete_user_alepox_in_populated_bd) {
    if (!db->openDB())
        EXPECT_TRUE(false);

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));

    EXPECT_TRUE(db->delete_user(username));

    EXPECT_THROW(db->get_user(username), UserDoesntExistException);
}


TEST_F(UsersDBFixture, test_edit_user_alepox_in_populated_bd) {
    if (!db->openDB())
        EXPECT_TRUE(false);

    string username = "alepox";
    Json::Value user = generate_user(username);
    EXPECT_TRUE(db->add_user(username, user));

    user["name"] = "marcelo";

    EXPECT_TRUE(db->edit_user(username, user));

    Json::Value userFromBD = db->get_user(username);

    //std::cout << userFromBD << std::endl;

    EXPECT_EQ(userFromBD["name"],Json::Value("marcelo"));
}

