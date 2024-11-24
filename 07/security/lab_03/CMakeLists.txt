cmake_minimum_required(VERSION 3.26)
project(lab_03 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab_03 main.c
        crypto.c
        crypto.h
        galois.h
        io.c
        io.h
        galois.c)
