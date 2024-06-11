cmake_minimum_required(VERSION 3.22.1)

project(
    csstruct
)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wuninitialized -Wextra")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON") 

add_executable(cstests test/tests.cpp)

target_include_directories(
    cstests
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/test
)


