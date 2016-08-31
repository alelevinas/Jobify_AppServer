cmake_minimum_required(VERSION 3.6)

project(foo)

set(HEADER_FILES Foo.h)
set(SOURCE_FILES Foo.cpp)


add_library(foo STATIC ${SOURCE_FILES} ${HEADER_FILES})