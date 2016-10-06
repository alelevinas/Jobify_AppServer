//
// Created by ale on 31/08/16.
//

#include <gtest/gtest.h>
#include <Foo.h>

class FooFixture : public ::testing::Test {

protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {

    }

public:
    FooFixture() : Test() {
        f = new Foo();
    }

    virtual ~FooFixture() {
        delete f;
    }

    Foo *f;
};


TEST_F(FooFixture, mi_test){
    EXPECT_EQ("Hola desde foo\n",f->hola());
}