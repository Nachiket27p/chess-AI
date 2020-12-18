cmake_minimum_required(VERSION 3.5)

project(chess-AI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(UNIX)
  add_executable(chess-AI
    main.cpp
    board.cpp
    board.h
    board.ui
    utils.h
    theme.cpp
    theme.h
    boardtile.cpp
    boardtile.h
  )
endif()

target_link_libraries(chess-AI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
