cmake_minimum_required(VERSION 3.6)
project(Jobify_AppServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp src/Foo.cpp src/Foo.h)
add_executable(Jobify_AppServer ${SOURCE_FILES})

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(Jobify_AppServer foo)
