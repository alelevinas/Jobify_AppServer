cmake_minimum_required(VERSION 2.8)
project(Jobify_AppServer)


#if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
set(CMAKE_CXX_FLAGS         "-Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused -DMG_ENABLE_THREADS -DMG_ENABLE_SSL -lssl -lcrypto -pthread")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -std=c++11")
#endif()


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES src/Foo.h src/mongoose-cpp/mongoose.h)  #lsoslas
set(SOURCE_FILES src/Foo.cpp src/mongoose-cpp/mongoose.c)


add_library(foo STATIC ${HEADER_FILES}  ${SOURCE_FILES} )


add_executable(Jobify_AppServer main.cpp ${SOURCE_FILES} ${HEADER_FILES})

include_directories(src)

#add_subdirectory(src)
add_subdirectory(test)

#target_link_libraries(Jobify_AppServer foo)
