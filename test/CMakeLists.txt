cmake_minimum_required(VERSION 2.8)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES foo_tests/basic_check.cpp foo_tests/FooFixture.cpp)

add_executable(run_tests main.cpp ${SOURCE_FILES} )

add_subdirectory(foo_tests)
add_subdirectory(lib/googletest)

target_link_libraries(run_tests gtest gtest_main foo)


#set(GOOGLETEST_ROOT ${PROJECT_SOURCE_DIR}/test/lib/googletest CACHE STRING "Google Test source root")
#
#include_directories(
#        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
#        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
#)
#
#set(GOOGLETEST_SOURCES
#        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
#        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
#        )

#foreach(_source ${GOOGLETEST_SOURCES})
#    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
#endforeach()
#
#add_library(googletest ${GOOGLETEST_SOURCES})
#
#add_executable(
#        unit_tests
#        ${SOURCE_FILES}
#)
#
#add_dependencies(unit_tests gtest gtest_main)
#
#target_link_libraries(
#        unit_tests
#        gtest
#        gtest_main
#        foo
#        pthread
#)
#
#include(CTest)
#enable_testing()
#
#add_test(unit ${PROJECT_BINARY_DIR}/unit_tests)