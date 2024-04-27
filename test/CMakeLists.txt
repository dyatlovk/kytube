cmake_minimum_required(VERSION 3.26)
project(qtest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES}
    network/request.hpp
    piped/search.hpp
)
target_link_libraries(${PROJECT_NAME} qtube::core)
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR} ${PROJECT_ROOT})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
