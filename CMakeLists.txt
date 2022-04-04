cmake_minimum_required(VERSION 3.21)
project(JSONSerializer C)

set(CMAKE_C_STANDARD 99)

add_executable(JSONSerializer JSON.h main.c)
