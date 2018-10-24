cmake_minimum_required(VERSION 3.8)
project(StemmerSemestralniPrace)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c HashMap.h HashMap.c)
add_executable(StemmerSemestralniPrace ${SOURCE_FILES})