cmake_minimum_required(VERSION 2.8)
project(Jobify_AppServer)


#if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
set(CMAKE_CXX_FLAGS         "-Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused -DMG_ENABLE_THREADS -DMG_ENABLE_SSL -lssl -lcrypto -pthread")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -std=c++11")
#endif()


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES src/Foo.h src/Server.h src/mongoose/mongoose.h src/Controlador.h src/ControladorPerfilUsuario.h)
set(SOURCE_FILES src/Foo.cpp src/Server.cpp src/mongoose/mongoose.c src/Controlador.cpp src/ControladorPerfilUsuario.cpp)


add_library(foo STATIC ${SOURCE_FILES} ${HEADER_FILES} )


add_executable(Jobify_AppServer main.cpp ${SOURCE_FILES} ${HEADER_FILES})

include_directories(src)

#add_subdirectory(src)
add_subdirectory(test)

#target_link_libraries(Jobify_AppServer foo)
