cmake_minimum_required(VERSION 3.8)
project(EagyBoxBlur)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES EagyBoxBlur.c)
add_executable(EagyBoxBlur ${SOURCE_FILES})