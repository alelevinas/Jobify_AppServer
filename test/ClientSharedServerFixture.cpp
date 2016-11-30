//
// Created by nicolas on 29/11/16.
//

#include <gtest/gtest.h>
#include <ClientSharedServer.h>
#include <DatabaseManager.h>

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

TEST_F(ClientSharedServerFixture, test_open_bd_is_ok){
    std::string obtenido;
    EXPECT_NO_THROW(obtenido = ssClient->getJobPositions());
    std::cout << "JOB POSITIIONS TEST: " << std::endl;
    std::cout << obtenido << std::endl;
}
