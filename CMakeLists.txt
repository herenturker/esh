cmake_minimum_required(VERSION 3.20)
project(esh LANGUAGES CXX RC)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC / clang-cl runtime (static CRT)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

file(GLOB_RECURSE SOURCES
    src/core/*.cpp
    src/env/*.cpp
    src/file/*.cpp
    src/process/*.cpp
    src/shell/*.cpp 
    src/system/*.cpp
    src/platform/*.cpp
    src/history/*.cpp
    src/consoleOperations/*.cpp
    src/execution/*.cpp
)

add_executable(esh ${SOURCES} resources.rc app.rc)

target_include_directories(esh PRIVATE 
    src/headers 
    src/external 
    src/env 
    src/file 
    src/process 
    src/shell 
    src/system 
    src/platform 
    src/history 
    src/consoleOperations
    src/execution
)

if (MSVC)
    target_compile_definitions(esh PRIVATE _CRT_SECURE_NO_WARNINGS)

    target_compile_options(esh PRIVATE
        /W4
        /permissive-
        /EHsc
    )

    target_link_options(esh PRIVATE /VERBOSE)
endif()

if (WIN32)
    target_link_libraries(esh PRIVATE kernel32 user32)
endif()
