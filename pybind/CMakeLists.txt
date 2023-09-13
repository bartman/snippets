cmake_minimum_required(VERSION 3.22)
project(MyProject)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pybind11 pybind11)

pybind11_add_module(my_module my_module.cpp)
