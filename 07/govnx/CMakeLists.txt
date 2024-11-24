cmake_minimum_required(VERSION 3.26)
project(govnx C)

set(CMAKE_C_STANDARD 23)

add_executable(govnx main.c
        net.c
        net.h
        errors.h
        log.c
        log.h)
