cmake_minimum_required(VERSION 3.20)
project(Kitsune VERSION 0.1.0 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
include(kitsune)

# Strip the project source directory from __FILE__ macros
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=)
endif()

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wvla
    -Wsign-conversion

    # Debug flags
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Debug>:-DDEBUG>

    # Release flags
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Release>:-march=native>
    $<$<CONFIG:Release>:-DNDEBUG>
)

# Default to debug build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(src)

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if (CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
    message(STATUS "Building tests...")
    add_subdirectory(test)
    message(STATUS "Building examples...")
    add_subdirectory(example)
endif()