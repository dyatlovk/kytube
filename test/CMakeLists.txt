cmake_minimum_required(VERSION 3.25)
project(qtest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(unit/core)

if (YOUTUBE_QT_GUI_ENABLED STREQUAL "ON")
    add_subdirectory(gui/qt)
endif()
