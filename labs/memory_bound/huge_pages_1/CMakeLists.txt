cmake_minimum_required(VERSION 3.3)

project(lab)

add_definitions(-D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)

string(REGEX MATCH "^(.*)[\\/]labs[\\/].*$" repo "${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_MATCH_1}/tools/labs.cmake)
