cmake_minimum_required(VERSION 3.26)
project(lab_01 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab_01 main.c
        enigma/enigma.h
        enigma/enigma.c
        file_io/file_io.c
        file_io/file_io.h
)

include_directories(file_io)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
