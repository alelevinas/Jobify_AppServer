//
// Created by nicolas on 28/11/16.
//

#include <gtest/gtest.h>
#include <DatabaseManager.h>
#include <exceptions/KeyDoesntExistException.h>

class ChatsDBFixture : public ::testing::Test {

protected:
    virtual void TearDown() {
        delete db;
    }

    virtual void SetUp() {
        db = new DatabaseManager("testing_accounts","testing_users","testing_sessions","testing_chats", "testing_images");
    }

public:
    ChatsDBFixture() : Test() {

    }

    virtual ~ChatsDBFixture() {
        system("rm -r testing_users");
        system("rm -r testing_sessions");
        system("rm -r testing_chats");
        system("rm -r testing_accounts");
        system("rm -r testing_images");
    }

    DatabaseManager* db;
};

TEST_F(ChatsDBFixture, test_open_bd_is_ok){
    EXPECT_TRUE(db->openDBs());
}

TEST_F(ChatsDBFixture, test_delete_new_msgs) {
    EXPECT_TRUE(db->openDBs());

    EXPECT_TRUE(db->add_msg("usuario1@correo1.com", "usuario2@correo2.com", "nuevo mensaje"));
    EXPECT_TRUE(db->add_msg("usuario1@correo1.com", "usuario2@correo2.com", "nuevo mensaje"));
    EXPECT_TRUE(db->add_msg("usuario1@correo1.com", "usuario2@correo2.com", "nuevo mensaje"));

    EXPECT_TRUE(db->delete_msg("usuario1@correo1.com", "usuario2@correo2.com", "1"));
    EXPECT_TRUE(db->delete_msg("usuario1@correo1.com", "usuario2@correo2.com", "1"));
    EXPECT_TRUE(db->delete_msg("usuario1@correo1.com", "usuario2@correo2.com", "0"));

    EXPECT_FALSE(db->delete_msg("usuario1@correo1.com", "usuario2@correo2.com", "0"));
}
TEST_F(ChatsDBFixture, test_delete_convs) {
    EXPECT_TRUE(db->openDBs());

    EXPECT_TRUE(db->add_msg("usuario1@correo1.com", "usuario2@correo2.com", "nuevo mensaje"));

    EXPECT_TRUE(db->delete_conv("usuario1@correo1.com", "usuario2@correo2.com"));
}

TEST_F(ChatsDBFixture, test_get_msgs) {
    EXPECT_TRUE(db->openDBs());

    Json::Value conversation;
    EXPECT_TRUE(db->add_msg("usuario1@correo1.com", "usuario2@correo2.com", "nuevo mensaje"));
    EXPECT_TRUE(db->get_conv("usuario1@correo1.com", "usuario2@correo2.com", &conversation));

    Json::Value userNotReal;
    EXPECT_THROW(db->get_convs("usuario1@correo1.com", &userNotReal), KeyDoesntExistException);
}