cmake_minimum_required(VERSION 4.2)
project(DoomHeadless C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -DNORMALUNIX -DLINUX -DHEADLESS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-but-set-variable")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pointer-to-int-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-const-variable")

file(GLOB SOURCES "*.c")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/doom_lib.c")

add_library(doomheadless STATIC ${SOURCES})

set_target_properties(doomheadless PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

target_link_libraries(doomheadless
    m
)

install(TARGETS doomheadless
    LIBRARY DESTINATION lib
)
