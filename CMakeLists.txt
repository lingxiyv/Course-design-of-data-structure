cmake_minimum_required(VERSION 4.1)
project(MineSweeper)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(MineSweeper
        main.cpp
        MineField.h
        MineField.cpp
        GameLogic.h
        GameLogic.cpp
        CustomButton.h
        CustomButton.cpp
        MainWindow.h
        MainWindow.cpp
        res.qrc
)

target_link_libraries(MineSweeper PRIVATE Qt6::Widgets)
