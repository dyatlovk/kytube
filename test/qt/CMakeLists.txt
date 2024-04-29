cmake_minimum_required(VERSION 3.26)
set(EXE_NAME qtube_test_qt)
project(${EXE_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
    UiTools
    REQUIRED
)

set(DOMAIN_NAME "kdyatlov")
set(CONFIG_NAME "YouTubeQt")
add_compile_definitions(DOMAIN_NAME="${DOMAIN_NAME}")
add_compile_definitions(CONFIG_NAME="${CONFIG_NAME}")

file(GLOB SOURCES
    main.cpp
    main/main_window.cpp
)
add_executable(${EXE_NAME} ${SOURCES})
target_include_directories(${EXE_NAME} PUBLIC ${EXTERNAL_DIR} ${PROJECT_ROOT})
target_link_libraries(${EXE_NAME}
    qtube::core
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::UiTools
)
add_test(NAME ${EXE_NAME} COMMAND ${CMAKE_BINARY_DIR}/test/qt/${EXE_NAME})
