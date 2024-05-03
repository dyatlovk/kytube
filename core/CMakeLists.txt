cmake_minimum_required(VERSION 3.25)
project(core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(${PROJECT_ROOT}/cmake/curl.cmake)

file(GLOB SOURCES
    network/request.cpp
    players/mpv.cpp
    datetime/datetime.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(qtube::core ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR} ${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBS})
