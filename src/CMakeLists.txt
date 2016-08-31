cmake_minimum_required(VERSION 2.8)

set(HEADER_FILES Foo.h)
set(SOURCE_FILES Foo.cpp)


add_library(foo STATIC ${SOURCE_FILES} ${HEADER_FILES})