set(BINARY passwordlib)

file(GLOB_RECURSE LIB_SRC LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${LIB_SRC})

add_library(${BINARY} ${LIB_SRC})