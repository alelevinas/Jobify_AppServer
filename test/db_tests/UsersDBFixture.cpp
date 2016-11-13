//
// Created by ale on 11/09/16.
//

#include <gtest/gtest.h>
#include <databases/UsersDB.h>
#include <exceptions/KeyDoesntExistException.h>
#include <exceptions/KeyAlreadyExistsException.h>
#include <DatabaseManager.h>

class UsersDBFixture : public ::testing::Test {

protected:
    virtual void TearDown() {
        delete db;
    }

    virtual void SetUp() {
        db = new DatabaseManager("testing_accounts","testing_users","testing_sessions","testing_chats");
    }

public:
    UsersDBFixture() : Test() {

    }

    virtual ~UsersDBFixture() {
        system("rm -r testing_users");
        system("rm -r testing_sessions");
        system("rm -r testing_chats");
        system("rm -r testing_accounts");
    }

    DatabaseManager* db;

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
        user["recommended_by"] = cont;
        user["chats"] = cont;

        return user;
    }
};

TEST_F(UsersDBFixture, test_open_bd_is_ok){
    EXPECT_TRUE(db->openDBs());
}

TEST_F(UsersDBFixture, test_add_user_alepox_in_empty_bd) {
    EXPECT_TRUE(db->openDBs());

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));
}

TEST_F(UsersDBFixture, test_get_user_alepox_in_empty_bd) {
    EXPECT_TRUE(db->openDBs());

    string username = "alepox";

    EXPECT_THROW(db->get_user(username), KeyDoesntExistException);
}



TEST_F(UsersDBFixture, test_add_user_alepox_twice_in_empty_bd) {
    EXPECT_TRUE(db->openDBs());

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));

    EXPECT_THROW(db->add_user(username, user), KeyAlreadyExistsException);
}

TEST_F(UsersDBFixture, test_get_user_alepox_in_populated_bd){
    EXPECT_TRUE(db->openDBs());

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));
    Json::Value userFromBD = db->get_user(username);

    EXPECT_EQ(userFromBD["username"],Json::Value(username));
}

TEST_F(UsersDBFixture, test_delete_invalid_user) {
    EXPECT_TRUE(db->openDBs());

    string username = "invalid_alepox";
    EXPECT_TRUE(db->delete_user(username));
}


TEST_F(UsersDBFixture, test_delete_user_alepox_in_populated_bd) {
    EXPECT_TRUE(db->openDBs());

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));

    EXPECT_TRUE(db->delete_user(username));

    EXPECT_THROW(db->get_user(username), KeyDoesntExistException);
}


TEST_F(UsersDBFixture, test_edit_user_alepox_in_populated_bd) {
    EXPECT_TRUE(db->openDBs());

    string username = "alepox";
    Json::Value user = generate_user(username);
    EXPECT_TRUE(db->add_user(username, user));

    user["name"] = "marcelo";

    EXPECT_TRUE(db->edit_user(username, user));

    Json::Value userFromBD = db->get_user(username);

    //std::cout << userFromBD << std::endl;

    EXPECT_EQ(userFromBD["name"],Json::Value("marcelo"));
}

TEST_F(UsersDBFixture, test_get_users_in_populated_bd) {
    EXPECT_TRUE(db->openDBs());

    string username = "alepox";
    Json::Value user = generate_user(username);

    EXPECT_TRUE(db->add_user(username, user));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    string users = db->get_users();

    std::cerr << users << std::endl;

    Json::Reader reader;
    Json::Value json_users;
    bool parsingSuccessful = reader.parse( users, json_users);
    if (!parsingSuccessful) {
        std::cerr << reader.getFormattedErrorMessages();
        EXPECT_TRUE(false);
    }

    EXPECT_EQ(json_users["users"][0]["username"],username); //no necesariamente deberia mantener el orden...
    EXPECT_EQ(json_users["users"][1]["username"],username2);
}

TEST_F(UsersDBFixture, test_recommend_user) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    EXPECT_TRUE(db->recommend_user(username1, username2));

    user2 = db->get_user(username2);

    //std::cerr << user2;

    EXPECT_EQ(user2["recommended_by"][0].asString(), username1);
}

TEST_F(UsersDBFixture, test_recommend_user_twice) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    EXPECT_TRUE(db->recommend_user(username1, username2));

    user2 = db->get_user(username2);

    EXPECT_EQ(user2["recommended_by"][0].asString(), username1);

    EXPECT_TRUE(db->recommend_user(username1, username2));

    user2 = db->get_user(username2);

    EXPECT_EQ(user2["recommended_by"][0].asString(), username1);

    //size is still 1
    EXPECT_EQ(user2["recommended_by"].size(),1);

}

TEST_F(UsersDBFixture, test_deRecommend_user) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    EXPECT_TRUE(db->recommend_user(username1, username2));

    user2 = db->get_user(username2);

    std::cerr << user2;

    EXPECT_EQ(user2["recommended_by"][0].asString(), username1);

    EXPECT_TRUE(db->deRecommend_user(username1,username2));

    user2 = db->get_user(username2);
    EXPECT_EQ(user2["recommended_by"].size(),0);
}

TEST_F(UsersDBFixture, test_deRecommend_user_that_wasnt_recommended) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    EXPECT_EQ(user2["recommended_by"].size(),0);

    EXPECT_FALSE(db->deRecommend_user(username1,username2));
}



