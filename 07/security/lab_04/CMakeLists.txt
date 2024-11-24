cmake_minimum_required(VERSION 3.26)
project(lab_04 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab_04 main.c
        bignum.c
        bignum.h
        io.c
        io.h
        io.h
        log.c
        log.h
        crypto.c
        crypto.h
)
