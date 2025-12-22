cmake_minimum_required(VERSION 3.15)
project(multithreading 
        VERSION 1.0
        DESCRIPTION "Multithreading Examples"
        LANGUAGES CXX)

# Use ccache if available (works on both macOS and WSL)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "Using ccache")
endif()

# Use mold linker on Linux, default on macOS
if(NOT APPLE)
    find_program(MOLD_LINKER mold)
    if(MOLD_LINKER)
        add_link_options(-fuse-ld=mold)
        message(STATUS "Using mold linker")
    endif()
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Debug builds - fast compilation
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -Wextra -g -O0)
else()
    add_compile_options(-Wall -Wextra -g -O2)
endif()

# Your source files...
set(SOURCES
    src/main.cpp
    src/ThreadGuard.cpp
    src/Vector.cpp
    src/bad_factorial.cpp
    src/shared_mutex_example.cpp
    src/singleton_example.cpp
    src/thread_local_example.cpp
    src/double_checked_example.cpp
    src/deadlock_example.cpp
    src/pre_seventeen_deadlock_example.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
