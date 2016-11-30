//
// Created by nicolas on 29/11/16.
//

#include <gtest/gtest.h>
#include <ClientSharedServer.h>

class ClientSharedServerFixture : public ::testing::Test {

protected:
    virtual void TearDown() {
        delete ssClient;
    }

    virtual void SetUp() {
        ssClient = new ClientSharedServer(URL, PORT);
    }

public:
    ClientSharedServerFixture() : Test() {

    }

    virtual ~ClientSharedServerFixture() {
    }

    ClientSharedServer* ssClient;
};

TEST_F(ClientSharedServerFixture, test_get_job_pos) {
    std::string obtenido;
    EXPECT_NO_THROW(obtenido = ssClient->getJobPositions());
    std::cout << "JOB POSITIIONS TEST: " << std::endl;
    std::cout << obtenido << std::endl << std::endl;
}

TEST_F(ClientSharedServerFixture, test_get_array_of_names_job_pos) {
    Json::Value obtenido;
    EXPECT_TRUE(ssClient->getNamesJobPositions(&obtenido));
    std::cout << "JOB POSITIIONS NAMES TEST: " << std::endl;
    std::cout << obtenido << std::endl << std::endl;
}

TEST_F(ClientSharedServerFixture, test_get_array_of_names_skills) {
    Json::Value obtenido;
    EXPECT_TRUE(ssClient->getNamesSkills(&obtenido));
    std::cout << "SKILLS NAMES TEST: " << std::endl;
    std::cout << obtenido << std::endl << std::endl;
}

TEST_F(ClientSharedServerFixture, test_get_array_of_names_categories) {
    Json::Value obtenido;
    EXPECT_TRUE(ssClient->getNamesCategories(&obtenido));
    std::cout << "CATEGORIES NAMES TEST: " << std::endl;
    std::cout << obtenido << std::endl << std::endl;
}