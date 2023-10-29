cmake_minimum_required(VERSION 3.20)
project(lab_09)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(lab_09 main.cpp mainwindow.cpp mainwindow.h mainwindow.ui task.cpp task.h)
target_link_libraries(lab_09
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

