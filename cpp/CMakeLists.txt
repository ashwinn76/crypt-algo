cmake_minimum_required(VERSION 3.10)

project(encryption)

include(gtest.cmake)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src/passwordlib)
add_subdirectory(src/passwords)

find_package (Threads)
add_subdirectory(tests)

message(STATUS "MY C++ STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "MY BUILD TYPE: ${CMAKE_BUILD_TYPE}")