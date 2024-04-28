cmake_minimum_required(VERSION 3.26)
project(qt)

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

add_executable(
    ${PROJECT_NAME}
    main.cpp
    controllers/main_window.cpp
    controllers/about.cpp
    controllers/log.cpp
    controllers/history.cpp
    controllers/preferences.cpp
    models/search.cpp
    models/log.cpp
    models/history.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR} ${PROJECT_ROOT})
target_link_libraries(${PROJECT_NAME}
    qtube::core
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::UiTools
)
