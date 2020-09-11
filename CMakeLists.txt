cmake_minimum_required(VERSION 3.10)

project(encryption)

set(CMAKE_CXX_STANDARD 20)

include(gtest.cmake)

include_directories(src)

find_package (Threads)
add_subdirectory(tests)
