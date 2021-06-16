set(BINARY tests)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

if (COVERAGE)
    target_compile_options(${BINARY} PRIVATE --coverage)
    target_link_libraries(${BINARY} PUBLIC gtest ${CMAKE_THREAD_LIBS_INIT} PRIVATE --coverage)
else()
    target_link_libraries(${BINARY} PUBLIC gtest ${CMAKE_THREAD_LIBS_INIT})
endif()

add_custom_command(TARGET ${BINARY} POST_BUILD COMMAND ${BINARY})
