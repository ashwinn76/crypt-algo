cmake_minimum_required(VERSION 3.10)

project(encryption)

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
	${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake)

if (COVERALLS)
	include(Coveralls)
	coveralls_turn_on_coverage()
endif()

include(gtest.cmake)

include_directories(src)

if (COVERALLS)
    coveralls_setup(
        "src/*.hpp"
        ${COVERALLS_UPLOAD}
        "${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake")
endif()

find_package (Threads)
add_subdirectory(tests)

message(STATUS "MY C++ STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "MY BUILD TYPE: ${CMAKE_BUILD_TYPE}")