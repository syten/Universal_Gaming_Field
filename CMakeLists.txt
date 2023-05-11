cmake_minimum_required(VERSION 3.24)
project(Universal_Gaming_Field)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Universal_Gaming_Field
        field.cpp
        field.h game_objects.cpp game_objects.h main_test.cpp interactions_caller.cpp interactions_caller.h)
