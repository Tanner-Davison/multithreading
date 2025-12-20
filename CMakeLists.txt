cmake_minimum_required(VERSION 3.15)
project(multithreading 
        VERSION 1.0
        DESCRIPTION "Multithreading Examples"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-fsanitize=thread)
add_compile_options(-g)
add_compile_options(-O1)
add_link_options(-fsanitize=thread)

# Source files

set(SOURCES
    src/main.cpp
    src/ThreadGuard.cpp
    src/Vector.cpp
    src/bad_factorial.cpp
    src/shared_mutex_example.cpp
    src/singleton_example.cpp
    src/thread_local_example.cpp
    src/double_checked_example.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


