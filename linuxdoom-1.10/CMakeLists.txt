# CMakeLists.txt

cmake_minimum_required(VERSION 4.2)
project(DoomHeadless C)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -DNORMALUNIX -DLINUX -DHEADLESS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-but-set-variable")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pointer-to-int-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-const-variable")

# Source files
file(GLOB SOURCES "*.c")

# Add doom_lib.c explicitly
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/doom_lib.c")

add_library(doomheadless STATIC ${SOURCES})

set_target_properties(doomheadless PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

target_link_libraries(doomheadless
    m  # Math library only
)

install(TARGETS doomheadless
    ARCHIVE DESTINATION lib
)