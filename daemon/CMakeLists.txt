cmake_minimum_required(VERSION 3.5.0)
project(daemon VERSION 0.1.0 LANGUAGES C CXX)

add_executable(daemon config.cpp daemon.cpp main.cpp)

