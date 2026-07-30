cmake_minimum_required(VERSION 3.22)

project(EmbeddedWorkspace C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall
    -Wextra
    -fdata-sections
    -ffunction-sections
)

add_subdirectory(BoardPackages)
add_subdirectory(EmbeddedFramework)
add_subdirectory(Projects/HelloFramework)