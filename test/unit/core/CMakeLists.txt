cmake_minimum_required(VERSION 3.25)
set(EXE_NAME qtube_test_core)
project(${EXE_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB SOURCES
    network/request.hpp
    piped/search.hpp
    piped/stream.hpp
    datetime/datetime.hpp
    sys/shell.hpp
)
add_executable(${EXE_NAME} ${SOURCES} main.cpp)
target_include_directories(${EXE_NAME} PUBLIC ${EXTERNAL_DIR} ${PROJECT_ROOT})
target_link_libraries(${EXE_NAME} qtube::core)
add_test(NAME ${EXE_NAME} COMMAND ${CMAKE_BINARY_DIR}/test/unit/core/${EXE_NAME})
