cmake_minimum_required(VERSION 3.0.0)
project(einstein VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES src/*)

add_executable(einstein ${SOURCES})

target_include_directories(einstein PUBLIC eigen)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
