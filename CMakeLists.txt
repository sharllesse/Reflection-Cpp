cmake_minimum_required(VERSION 3.29)

project(Reflection_C++)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME} 
    PRIVATE 
        "src/Main.cpp"
        "src/Reflect.cpp"
        "include/Reflect.h"
        "pch/Pch.h")

target_include_directories(${CMAKE_PROJECT_NAME} 
    PRIVATE
        "include"
        "pch")

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_23)

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE "pch/Pch.h")