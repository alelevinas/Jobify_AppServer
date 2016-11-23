//
// Created by ale on 31/08/16.
//

#include <log/easylogging++.h>
#include "gtest/gtest.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}