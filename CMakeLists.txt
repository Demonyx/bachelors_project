cmake_minimum_required(VERSION 3.26.4)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_EXTENSIONS TRUE)
set (CMAKE_BUILD_TYPE Debug)

project(bachelors_project)
add_executable(btree src/btree.cpp)

