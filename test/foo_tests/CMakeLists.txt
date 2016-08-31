include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runFooTests basic_check.cpp FooFixture.cpp)

target_link_libraries(runFooTests gtest gtest_main)
target_link_libraries(runFooTests foo)