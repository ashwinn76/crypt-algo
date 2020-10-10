cmake_minimum_required(VERSION 3.10)

project(encryption)

include(gtest.cmake)

include_directories(src)

set(COVERAGE OFF CACHE BOOL "Coverage")

find_package (Threads)
add_subdirectory(tests)

message(STATUS "MY C++ STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "MY BUILD TYPE: ${CMAKE_BUILD_TYPE}")