cmake_minimum_required(VERSION 2.8)
project(Jobify_AppServer)


set(CMAKE_CXX_FLAGS         "-Wall -Wno-unknown-pragmas -Wno-sign-compare  -Wwrite-strings -Wno-unused -DMG_ENABLE_THREADS -DMG_ENABLE_SSL -lssl -lcrypto -pthread")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -std=c++11 -lleveldb -lpthread -lsnappy -lcryptopp -LLIBDIR")# -L/usr/local/lib") #-LLIBDIR



#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Compilo la libreria de mongoose-cpp
set (MONGOOSE_CPP "${PROJECT_SOURCE_DIR}/src/mongoose-cpp")

set (MONGOOSE_SOURCES
        ${MONGOOSE_CPP}/mongoose.c
        ${MONGOOSE_CPP}/Utils.cpp
        ${MONGOOSE_CPP}/Controller.cpp
        ${MONGOOSE_CPP}/Mutex.cpp
        ${MONGOOSE_CPP}/Request.cpp
        ${MONGOOSE_CPP}/Response.cpp
        ${MONGOOSE_CPP}/Server.cpp
        ${MONGOOSE_CPP}/Session.cpp
        ${MONGOOSE_CPP}/Sessions.cpp
        ${MONGOOSE_CPP}/StreamResponse.cpp
        ${MONGOOSE_CPP}/UploadFile.cpp
        ${MONGOOSE_CPP}/WebController.cpp
        ${MONGOOSE_CPP}/JsonResponse.cpp
        ${MONGOOSE_CPP}/JsonController.cpp
        ${MONGOOSE_CPP}/WebSocket.cpp
        ${MONGOOSE_CPP}/WebSockets.cpp
        )

include_directories ("${MONGOOSE_CPP}")

# Compiling library
add_library (mongoose ${MONGOOSE_SOURCES})
target_link_libraries (mongoose ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})

#Compilo la libreria de json
set (JSONCPP_DIR "${PROJECT_SOURCE_DIR}/src/jsoncpp")
include_directories ("${JSONCPP_DIR}")

add_library (json ${JSONCPP_DIR}/jsoncpp.cpp)
target_link_libraries (mongoose json leveldb)


#set (JWTCPP_DIR "${PROJECT_SOURCE_DIR}/src/jwt")
#add_library (jwtcpp ${JWTCPP_DIR}/jwt.cpp ${JWTCPP_DIR}/utils.cpp)

#LevelDB
#include_directories("/usr/include/leveldb")


#set(HEADER_FILES src/Foo.h src/mongoose-cpp/mongoose.h)  #lsoslas
set(SOURCE_FILES
        src/Foo.cpp
        src/DatabaseManager.cpp
        src/databases/UsersDB.cpp src/databases/UsersDB.h
        src/databases/ChatsDB.cpp src/databases/ChatsDB.h
        src/JobifyServer.cpp src/JobifyServer.h
        src/ProfileController.cpp src/ProfileController.h
        src/exceptions/KeyDoesntExistException.h
        src/exceptions/KeyAlreadyExistsException.h
        src/databases/SessionsDB.cpp src/databases/SessionsDB.h
        src/databases/DB.cpp src/databases/DB.h
        src/databases/utils.cpp
        src/SessionManager.cpp src/SessionManager.h
        src/exceptions/TokenInvalidException.h
        src/databases/AccountsDB.cpp src/databases/AccountsDB.h
        src/exceptions/InvalidUsernamePasswordException.h
        src/log/easylogging++.h
        src/ApiError.h src/databases/ImagesDB.cpp src/databases/ImagesDB.h
        src/ClientSharedServer.cpp src/NotificationSender.cpp src/NotificationSender.h)

add_library(foo STATIC ${HEADER_FILES}  ${SOURCE_FILES} )


add_executable(Jobify_AppServer main.cpp ${SOURCE_FILES} ${HEADER_FILES} src/ChatController.cpp src/ChatController.h src/ClientSharedServer.cpp src/ClientSharedServer.h src/SharedServerController.cpp src/SharedServerController.h src/HaversineDistance.h)
target_link_libraries (Jobify_AppServer leveldb mongoose json cryptopp restclient-cpp)

include_directories(src)

#add_subdirectory(src)
add_subdirectory(test)

#target_link_libraries(Jobify_AppServer foo)
