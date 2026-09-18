cmake_minimum_required(VERSION 3.20)
project(Desafio1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Desafio1
    src/main.cpp
    src/lib.cpp
    src/utilities.cpp

    include/lib.hpp
    include/utilities.hpp
)

target_include_directories(Desafio1 PRIVATE include)
