cmake_minimum_required(VERSION 3.16)
project(Esh LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC / clang-cl runtime (static CRT)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

file(GLOB_RECURSE SOURCES
    Esh/src/*.cpp
)

add_executable(Esh ${SOURCES})

target_include_directories(Esh PRIVATE Esh/src/headers)

if (MSVC)
    target_compile_definitions(Esh PRIVATE _CRT_SECURE_NO_WARNINGS)

    target_compile_options(Esh PRIVATE
        /W4
        /permissive-
        /EHsc
    )

    target_link_options(Esh PRIVATE /VERBOSE)
endif()

if (WIN32)
    target_link_libraries(Esh PRIVATE kernel32 user32)
endif()
