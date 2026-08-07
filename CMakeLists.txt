cmake_minimum_required(VERSION 3.16)

project(MyQtApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core Sql)

add_executable(MyQtApp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    db.cpp
    db.h
)

target_link_libraries(MyQtApp PRIVATE Qt5::Widgets)
target_link_libraries(MyQtApp PRIVATE Qt5::Core Qt5::Sql)
