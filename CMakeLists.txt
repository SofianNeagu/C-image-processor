cmake_minimum_required(VERSION 4.1)
project(Imageproject C)

set(CMAKE_C_STANDARD 11)

add_executable(Imageproject main.c
        Imageprocessing.c
        Imageprocessing.h)
