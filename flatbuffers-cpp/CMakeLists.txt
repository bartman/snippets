cmake_minimum_required(VERSION 3.5.1)
project(flat)

SET(CMAKE_CXX_FLAGS_RELEASE "--std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} -g -DDEBUG")
set(CMAKE_VERBOSE_MAKEFILE ON)


add_custom_target(message_generated.h
        COMMAND flatc --cpp ${CMAKE_CURRENT_SOURCE_DIR}/message.fbs
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/message.fbs
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/message_generated.h
        )

add_executable(flat flat_main.cpp )
add_dependencies(flat message_generated.h)
target_include_directories(flat PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(flat rt flatbuffers)

#set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabi-gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabi-g++)

#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
