cmake_minimum_required(VERSION 3.26)
project(lab_05 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab_05 main.c
        file.c
        file.h
        tree.c
        tree.h
        huffman.c
        huffman.h)
