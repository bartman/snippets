cmake_minimum_required(VERSION 3.22)
project(MyProject)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
)
FetchContent_GetProperties(pybind11)
FetchContent_Populate(pybind11)
add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})

pybind11_add_module(my_module my_module.cpp)
