cmake_minimum_required(VERSION 3.24)
project(zadanie4 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -lm")

add_executable(zadanie4 main.c
        monopoly.h
        monopoly.c)
target_include_directories(zadanie4 PRIVATE ${PROJECT_SOURCE_DIR}/include)
