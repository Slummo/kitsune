cmake_minimum_required(VERSION 4.0)
project(kitsune VERSION 1.0.0 LANGUAGES C)

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
    # -Wpedantic
    -std=c99

    # Debug flags
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Debug>:-fsanitize=address>
    $<$<CONFIG:Debug>:-DDEBUG>

    # Release flags
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Release>:-march=native>
    $<$<CONFIG:Release>:-DNDEBUG>
)

add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)

# Default to debug build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_library(kitsune INTERFACE)
target_include_directories(kitsune INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(kitsune INTERFACE m)

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if (CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
  message(STATUS "Building tests...")
  add_subdirectory(test)
  message(STATUS "Building examples...")
  add_subdirectory(example)
endif()
