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
        db = new DatabaseManager("testing_accounts","testing_users","testing_sessions","testing_chats", "testing_images");
    }

public:
    UsersDBFixture() : Test() {

    }

    virtual ~UsersDBFixture() {
        system("rm -r testing_users");
        system("rm -r testing_sessions");
        system("rm -r testing_chats");
        system("rm -r testing_accounts");
        system("rm -r testing_images");
    }

    DatabaseManager* db;

    Json::Value add_job_exp(Json::Value &user, const char* years, const char* company, const char* description,
                            const char* pos_name, const char* pos_description, const char* pos_category) {

        Json::Value job;
        job["years"] = years;
        job["company"] = company;
        job["description"] = description;

//        Json::Value pos;
//        pos["name"] = pos_name;
//        pos["description"] = pos_description;
//        pos["category"] = pos_category;

        //job["position"] = pos;
        job["position"] = pos_name;

        user["previous_exp"].append(job);
        return user;
    }

    Json::Value add_skill(Json::Value &user, const char* name, const char* description, const char* category) {
//        Json::Value skill;
//        skill["name"] = name;
//        skill["description"] = description;
//        skill["category"] = category;
//
//        user["skills"].append(skill);
        user["skills"].append(name);
//        return skill;
        return user["skills"];
    }

    Json::Value generate_user(string &username) {
        Json::Value user;
        user["username"] = username;
        user["name"] = "Alejandro Pablo Levinas";
        user["gender"] = "male";
        user["coordenates"] = "0:0";
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

//        add_skill(user, "C","C programming language","Software");
//        add_skill(user, "C++","C++ programming language","Software");
//        add_skill(user, "R","R programming language","Software");


        Json::Value exp(Json::arrayValue);
        user["previous_exp"] = exp;
//        add_job_exp(user, "2006-2009", "NASA", "Desarrollador en lenguaje R para analizar......", "Docente", "Profesor dicta clases, etc.", "Education");
//        add_job_exp(user, "2010-actualidad", "UBA", "Docente de la materia Taller 2", "Docente", "Profesor dicta clases, etc.", "Education");


        Json::Value job1;
        job1["years"] = "2006-2009";
        job1["company"] = "NASA";
        job1["position"] = "developer";
        //job1["position"] = add_job_position(user, "developer", "Software dev....", "Software");
        job1["description"] = "Desarrollador en lenguaje R para analizar......";

        Json::Value job2;
        job2["years"] = "2010-actualidad";
        job2["company"] = "UBA";
        job2["position"] = "Docente";
        //job2["position"] = add_job_position(user, "Docente", "Profesor dicta clases, etc.", "Education");
        job2["description"] = "Docente de la materia Taller 2";

        exp.append(job1);
        exp.append(job2);
        user["previous_exp"] = exp;

        Json::Value cont(Json::arrayValue);
        Json::Value recc(Json::arrayValue);
        Json::Value chats(Json::arrayValue);
        user["contacts"] = cont;

      //  recc.append("ale");
       // recc.append("peter");
        user["recommended_by"] = recc;


        user["chats"] = chats;

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

    Json::Value users(Json::arrayValue);

    bool status = db->get_users(users);

    if (!status) {
        std::cerr << "Parser error";
        EXPECT_TRUE(false);
    }

    EXPECT_EQ(users["users"][0]["username"],username); //no necesariamente deberia mantener el orden...
    EXPECT_EQ(users["users"][1]["username"],username2);
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

TEST_F(UsersDBFixture, test_add_contact) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    EXPECT_TRUE(db->addContact(username1, username2));

    user1 = db->get_user(username1);
    user2 = db->get_user(username2);

    EXPECT_EQ(user1["contacts"][0].asString(), username2);
    EXPECT_EQ(user2["contacts"][0].asString(), username1);
}

TEST_F(UsersDBFixture, test_add_contact_twice) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    EXPECT_TRUE(db->addContact(username1, username2));

    user1 = db->get_user(username1);
    user2 = db->get_user(username2);

    EXPECT_EQ(user1["contacts"][0].asString(), username2);
    EXPECT_EQ(user2["contacts"][0].asString(), username1);

    EXPECT_TRUE(db->addContact(username2, username1));

    user1 = db->get_user(username1);
    user2 = db->get_user(username2);

    EXPECT_EQ(user1["contacts"][0].asString(), username2);
    EXPECT_EQ(user2["contacts"][0].asString(), username1);

    EXPECT_EQ(user1["contacts"].size(), 1);
    EXPECT_EQ(user2["contacts"].size(), 1);
}

TEST_F(UsersDBFixture, test_remove_contact) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    EXPECT_TRUE(db->addContact(username1, username2));

    user1 = db->get_user(username1);
    user2 = db->get_user(username2);

    EXPECT_EQ(user1["contacts"][0].asString(), username2);
    EXPECT_EQ(user2["contacts"][0].asString(), username1);

    EXPECT_TRUE(db->removeContact(username1,username2));

    user1 = db->get_user(username1);
    user2 = db->get_user(username2);

    EXPECT_EQ(user1["contacts"].size(), 0);
    EXPECT_EQ(user2["contacts"].size(), 0);
}

TEST_F(UsersDBFixture, test_get_users_job_carpintero) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);

    user1["recommended_by"].append("ale");
    user1["recommended_by"].append("gabi");
    add_job_exp(user1, "2006-2009", "NASA", "Desarrollador en lenguaje R para analizar......", "Carpintero", "Profesor dicta clases, etc.", "Education");

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    user2["recommended_by"].append("pepe");
    EXPECT_TRUE(db->add_user(username2, user2));

    Json::Value users;
    db->get_users_by("", 10, "Carpintero", "", users, 100,"0:0");
    std::cerr << "\n------------------RESULTADO FILTRADO--------------\n"
         << users;

    EXPECT_EQ(users[0], user1);
}

TEST_F(UsersDBFixture, test_get_users_skill_Gtest) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);

    add_skill(user1, "Google Test","Testing framework for C/C++","Software");

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    Json::Value users;
    db->get_users_by("", 10, "", "Google Test", users, 100,"0:0");

    std::cerr << "\n------------------RESULTADO FILTRADO--------------\n"
              << users;

    EXPECT_EQ(users[0], user1);
}

TEST_F(UsersDBFixture, test_get_users_ordered_by_recommendations) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);


    add_job_exp(user1, "2006-2009", "NASA", "Desarrollador en lenguaje R para analizar......", "Carpintero", "Profesor dicta clases, etc.", "Education");

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    user2["recommended_by"].append("pepe");
    user2["recommended_by"].append("ale");
    user2["recommended_by"].append("gabi");
    EXPECT_TRUE(db->add_user(username2, user2));

    string username3 = "gabi";
    Json::Value user3 = generate_user(username3);
    user3["recommended_by"].append("ale");

    EXPECT_TRUE(db->add_user(username3, user3));

    Json::Value users_ordered_by_rec;
    db->get_users_by("recommended_by", 10, "", "", users_ordered_by_rec, 100,"0:0");

    std::cerr << "\n------------------RESULTADO FILTRADO--------------\n"
              << users_ordered_by_rec;

    EXPECT_EQ(users_ordered_by_rec[0], user2);
    EXPECT_EQ(users_ordered_by_rec[1], user3);
    EXPECT_EQ(users_ordered_by_rec[2], user1);
}

TEST_F(UsersDBFixture, test_get_top_ten_users_ordered_by_recommendations) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);
    user1["recommended_by"].append("ale");
    user1["recommended_by"].append("ale");

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    user2["recommended_by"].append("pepe");
    user2["recommended_by"].append("ale");
    user2["recommended_by"].append("gabi");
    user2["recommended_by"].append("ale");
    EXPECT_TRUE(db->add_user(username2, user2));

    string username3 = "gabi";
    Json::Value user3 = generate_user(username3);
    user3["recommended_by"].append("ale");
    user3["recommended_by"].append("ale");
    user3["recommended_by"].append("ale");


    EXPECT_TRUE(db->add_user(username3, user3));


    std::vector <Json::Value> users;
    users.push_back(user1);
    users.push_back(user2);
    users.push_back(user3);

    for (int i=0; i<20; i++) {
        string username = "juan_" + std::to_string(i);
        Json::Value user = generate_user(username);
        users.push_back(user);
        EXPECT_TRUE(db->add_user(username, user));
    }

    EXPECT_EQ(23, users.size());

    Json::Value users_ordered_by_rec;
    db->get_users_by("recommended_by", 10, "", "", users_ordered_by_rec, 100,"0:0");

    std::cerr << "\n------------------RESULTADO FILTRADO--------------\n"
              << users_ordered_by_rec;

    EXPECT_EQ(10, users_ordered_by_rec.size());
    EXPECT_EQ(users_ordered_by_rec[0], user2);
    EXPECT_EQ(users_ordered_by_rec[1], user3);
    EXPECT_EQ(users_ordered_by_rec[2], user1);
}

TEST_F(UsersDBFixture, test_filter_users_by_Ndistance) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox11";
    Json::Value user1 = generate_user(username1);
    user1["coordenates"] = ("100:100");

    string username2 = "marcelo11";
    Json::Value user2 = generate_user(username2);
    user2["coordenates"] = ("100:120");

    string username3 = "gabi11";
    Json::Value user3 = generate_user(username3);
    user3["coordenates"] = ("100:9999999999");

    db->add_user(username1,user1);
    db->add_user(username2,user2);
    db->add_user(username3,user3);

    Json::Value users;
    db->get_users(users);
    Json::Value result(Json::arrayValue);
    result.swapPayload(users["users"]);

    EXPECT_EQ(3,result.size());
    db->filter_pos(result, 100, "100:100");
    EXPECT_EQ(1,result.size());
}

TEST_F(UsersDBFixture, test_sort_users_by_distance) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox11";
    Json::Value user1 = generate_user(username1);
    user1["coordenates"] = ("100:100");

    string username2 = "marcelo11";
    Json::Value user2 = generate_user(username2);
    user2["coordenates"] = ("100:120");

    string username3 = "gabi11";
    Json::Value user3 = generate_user(username3);
    user3["coordenates"] = ("100:9999999999");

    db->add_user(username1,user1);
    db->add_user(username2,user2);
    db->add_user(username3,user3);

    Json::Value users;
    db->get_users(users);
    Json::Value result(Json::arrayValue);
    result.swapPayload(users["users"]);

    std::cerr << "-------ANTES DE ORDENAR POR DISTANCIA---------"
            << result;
    EXPECT_EQ(3,result.size());
    db->sort_by_distance(result, "100:120");
    EXPECT_EQ(3,result.size());
    std::cerr << "-------DESPUES DE ORDENAR POR DISTANCIA---------"
              << result;
}


TEST_F(UsersDBFixture, test_get_contacts_info) {
    EXPECT_TRUE(db->openDBs());

    string username1 = "alepox";
    Json::Value user1 = generate_user(username1);

    EXPECT_TRUE(db->add_user(username1, user1));

    string username2 = "marcelo";
    Json::Value user2 = generate_user(username2);

    EXPECT_TRUE(db->add_user(username2, user2));

    string username3 = "pepe";
    Json::Value user3 = generate_user(username3);

    EXPECT_TRUE(db->add_user(username3, user3));

    EXPECT_TRUE(db->addContact(username1, username2));
    EXPECT_TRUE(db->addContact(username1, username3));

    user1 = db->get_user(username1);
    user2 = db->get_user(username2);
    user3 = db->get_user(username3);

    Json::Value contacts(Json::arrayValue);
    db->get_user_contacts(username1, contacts);

    EXPECT_EQ(contacts[0], user2);
    EXPECT_EQ(contacts[1], user3);
}