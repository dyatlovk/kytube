cmake_minimum_required(VERSION 3.26)
project(qtest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(core)
add_subdirectory(qt)
