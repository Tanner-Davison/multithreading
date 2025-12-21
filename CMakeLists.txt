cmake_minimum_required(VERSION 3.15)
project(multithreading 
        VERSION 1.0
        DESCRIPTION "Multithreading Examples"
        LANGUAGES CXX)

# Detect and set compiler
if(NOT CMAKE_CXX_COMPILER)
    # Try to find clang++-18 first
    find_program(CLANG18 clang++-18)
    if(CLANG18)
        set(CMAKE_CXX_COMPILER clang++-18)
    else()
        # Fall back to default system compiler (clang++ on macOS, g++ on WSL)
        if(APPLE)
            set(CMAKE_CXX_COMPILER clang++)
        else()
            # On Linux/WSL, try g++ or clang++
            find_program(GPP g++)
            find_program(CLANG clang++)
            if(GPP)
                set(CMAKE_CXX_COMPILER g++)
            elseif(CLANG)
                set(CMAKE_CXX_COMPILER clang++)
            endif()
        endif()
    endif()
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-fsanitize=address -g -O1)
add_link_options(-fsanitize=address)
# Compiler-specific flags
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     add_compile_options(-fsanitize=thread -g -O1)
#     add_link_options(-fsanitize=thread)
# elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#     add_compile_options(-fsanitize=thread -g -O1)
#     add_link_options(-fsanitize=thread)
# endif()

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
    src/deadlock_example.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Print which compiler is being used
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")
