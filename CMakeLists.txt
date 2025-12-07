cmake_minimum_required(VERSION 3.15)
project(multithreading 
        VERSION 1.0
        DESCRIPTION "Multithreading Examples"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Source files
set(SOURCES
    src/main.cpp
    src/ThreadGaurd.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


