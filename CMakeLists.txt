cmake_minimum_required(VERSION 3.26)
project(YouTubeQt)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -Wall -Wextra -ggdb3")
endif ()

find_package(CURL REQUIRED)
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        UiTools
        REQUIRED)

#qt5_wrap_ui(UIS_HDRS forms/main.ui forms/about.ui)
set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(DOMAIN_NAME "kdyatlov")

configure_file(Config.hpp.in "${PROJECT_ROOT}/Config.hpp" @ONLY)

option(YOUTUBE_QT_BUILD_TESTS "Build unit tests" OFF)
if (YOUTUBE_QT_BUILD_TESTS)
    add_subdirectory(test)
    enable_testing()
endif ()

add_executable(${PROJECT_NAME}
        main.cpp
        gui/MainWindow.cpp
        gui/AboutDialog.cpp
        gui/Preferences.cpp
        gui/Log.cpp
        model/VideoModel.cpp
        model/LogModel.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR})

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::UiTools
        ${CURL_LIBRARIES}
)

