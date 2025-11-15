cmake_minimum_required(VERSION 3.10)
project(Lang C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(Lang src/main.c)
target_include_directories(Lang PRIVATE src)