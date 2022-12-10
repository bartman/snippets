cmake_minimum_required(VERSION 3.5.1)
project(flat)

SET(CMAKE_C_FLAGS_RELEASE "--std=c11")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE} -g -DDEBUG")
set(CMAKE_VERBOSE_MAKEFILE ON)


add_custom_target(message_generated.h
        COMMAND flatcc --outfile ${CMAKE_CURRENT_BINARY_DIR}/message_generated.h
                       -a ${CMAKE_CURRENT_SOURCE_DIR}/message.fbs
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/message.fbs
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/message_generated.h
        )

# this is where I installed my flatcc headers and lib
set(FLATCC_INCLUDE_DIR /home/bart/usr/include/)
set(FLATCC_LIB_DIR /home/bart/usr/lib)
set(FLATCC_STATIC_LIB ${FLATCC_LIB_DIR}/libflatccrt.a)

add_executable(flat flat_main.c )
add_dependencies(flat message_generated.h)
target_include_directories(flat PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        ${FLATCC_INCLUDE_DIR})
target_link_libraries(flat ${FLATCC_STATIC_LIB})

#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
