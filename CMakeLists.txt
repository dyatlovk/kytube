cmake_minimum_required(VERSION 3.25)
project(YouTubeQt)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Set default build type
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build debug")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -Wall -Wextra -ggdb3")
endif ()

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)

configure_file(Config.hpp.in "${PROJECT_ROOT}/Config.hpp" @ONLY)

set(YOUTUBE_QT_GUI_ENABLED "ON" CACHE STRING "Enable GUI. Disable if you only want to run unit tests or build core library.")
set_property(CACHE YOUTUBE_QT_GUI_ENABLED PROPERTY STRINGS "ON" "OFF")

add_subdirectory(core)
if (YOUTUBE_QT_GUI_ENABLED STREQUAL "ON")
    add_subdirectory(ui/qt)
endif()

option(YOUTUBE_QT_BUILD_TESTS "Build tests" OFF)
if (YOUTUBE_QT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
