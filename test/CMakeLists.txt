cmake_minimum_required(VERSION 2.8)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES
        foo_tests/basic_check.cpp
        foo_tests/FooFixture.cpp
        db_tests/UsersDBFixture.cpp
        db_tests/SessionsDBFixture.cpp
        db_tests/SessionManagerFixture.cpp db_tests/ChatsDBFixture.cpp)


add_executable(run_tests main.cpp ${SOURCE_FILES} )

add_subdirectory(foo_tests)
add_subdirectory(lib/googletest)
include_directories(db_tests)


target_link_libraries(run_tests gtest gtest_main foo mongoose json leveldb cryptopp)