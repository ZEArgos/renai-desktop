cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Os -g -Wall -Werror -Wpedantic)
add_library("error" Error.c)