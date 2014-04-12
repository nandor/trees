cmake_minimum_required(VERSION 2.8)
project(trees)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Wextra")
add_executable(trees Test.cc)
