cmake_minimum_required(VERSION 3.10)

project(encryption)

include(gtest.cmake)

set(COVERAGE OFF CACHE BOOL "Coverage")

find_package (Threads)

file(GLOB_RECURSE HDRS LIST_DIRECTORIES false *.hpp)

add_executable(algo-tests tests/main.cpp ${HDRS})

if (COVERAGE)
    target_compile_options(algo-tests PRIVATE --coverage)
    target_link_libraries(algo-tests PUBLIC gtest ${CMAKE_THREAD_LIBS_INIT} PRIVATE --coverage)
else()
	target_link_libraries(algo-tests gtest ${CMAKE_THREAD_LIBS_INIT})
endif()

message(STATUS "MY C++ STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "MY BUILD TYPE: ${CMAKE_BUILD_TYPE}")