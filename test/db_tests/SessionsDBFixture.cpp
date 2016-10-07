//
// Created by ale on 3/10/16.
//
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <gtest/gtest.h>
#include <databases/UsersDB.h>
#include <databases/SessionsDB.h>

class SessionsDBFixture : public ::testing::Test {

protected:
    virtual void TearDown() {
        delete db;
    }

    virtual void SetUp() {
        db = new SessionsDB(db_name);
    }

public:
    SessionsDBFixture() : Test() {

    }

    virtual ~SessionsDBFixture() {
        system("rm -r testing_sessions");
    }

    SessionsDB *db;
    string db_name = "testing_sessions";

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


TEST_F(SessionsDBFixture, test_open_bd_is_ok) {
    EXPECT_TRUE(db->openDB());
}


TEST_F(SessionsDBFixture, test_add_token) {
    if (!db->openDB())
        EXPECT_TRUE(false);

    string username = "alepox";
    string password = "123456";
/*    string usr_pass = username + ":" + password;
    Json::Value user = generate_user(username);

    using namespace CryptoPP;
    string encodedUsrNPass;
    StringSource(usr_pass, true, new Base64Encoder(new StringSink(encodedUsrNPass)));

    std::cout << "Base64: " << encodedUsrNPass << std::endl;

    CryptoPP::Weak::MD5 hash;
    string digest;

    StringSource s(encodedUsrNPass, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));

    std::cout << "MD5: " << digest << std::endl;*/


    EXPECT_TRUE(db->add_session(username, password));
}