cmake_minimum_required(VERSION 3.29)

project(Reflection_Cpp)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME} 
    PRIVATE 
        "src/main.cpp"
        "include/reflect.h"
        "include/variable_field.h"
        "include/function_field.h"
        "include/compile_string.h"
)

target_include_directories(${CMAKE_PROJECT_NAME} 
    PRIVATE
        "include")

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_23)