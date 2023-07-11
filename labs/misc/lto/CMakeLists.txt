cmake_minimum_required(VERSION 3.3)

project(lab)

set(VALIDATE_ARGS "${CMAKE_CURRENT_SOURCE_DIR}/golden.ppm")

# seems only this is ok
# set(CMAKE_C_FLAGS "-flto ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-flto ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-flto ${CMAKE_EXE_LINKER_FLAGS} ")

string(REGEX MATCH "^(.*)[\\/]labs[\\/].*$" repo "${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_MATCH_1}/tools/labs.cmake)
