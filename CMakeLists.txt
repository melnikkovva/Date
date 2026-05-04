cmake_minimum_required(VERSION 3.10)
project(CDateTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CDateTests
    Tests.cpp
    CDate.cpp
)

target_include_directories(CDateTests PRIVATE ${CMAKE_SOURCE_DIR})

enable_testing()
add_test(NAME CDateTests COMMAND CDateTests)