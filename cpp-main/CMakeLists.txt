cmake_minimum_required(VERSION 3.5.1)
project(cppmain)

SET(CMAKE_CXX_FLAGS_RELEASE "--std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} -g -DDEBUG")
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(cppmain main.cpp )
