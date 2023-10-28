cmake_minimum_required(VERSION 3.20)
project(lab_02 C)

set(CMAKE_C_STANDARD 99)

MATH(EXPR stack_size "1024 * 1024 * 1024")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-z,-stack-size=${stack_size}")

include_directories(./inc)
include_directories(./src)

set(SOURCE_FILES
        src/main.c
        src/error.c
        inc/error.h
        inc/flat.h
        src/flat.c inc/mystring.h src/mystring.c inc/flats_db.h src/flats_db.c inc/table.h src/table.c src/ui.c inc/ui.h src/benchmark.c inc/benchmark.h)

add_executable(lab_02 ${SOURCE_FILES})
