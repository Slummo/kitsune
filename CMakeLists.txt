cmake_minimum_required(VERSION 4.0)
project(kitsune VERSION 1.0.0 LANGUAGES C)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_KS TRUE)
else()
    set(IS_KS FALSE)
endif()

if (IS_KS)
    # Strip the project source directory from __FILE__ macros
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
      add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=)
    endif()

    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wsign-conversion
        -std=c99

        # Debug flags
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-fsanitize=address>
        $<$<CONFIG:Debug>:-fsanitize=leak>
        $<$<CONFIG:Debug>:-fsanitize=undefined>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        $<$<CONFIG:Debug>:-DDEBUG>

        # Release flags
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-march=native>
        $<$<CONFIG:Release>:-DNDEBUG>
    )

    add_link_options(
        $<$<CONFIG:Debug>:-fsanitize=address>
        $<$<CONFIG:Debug>:-fsanitize=leak>
        $<$<CONFIG:Debug>:-fsanitize=undefined>
    )

    # Default to debug build
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE Debug)
    endif()
endif()

find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(extern)

add_library(ks INTERFACE)
target_include_directories(ks INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ks INTERFACE m OpenMP::OpenMP_C ks::glad OpenGL::GL glfw)
add_library(ks::lib ALIAS ks)

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if (IS_KS AND CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
    message(STATUS "Building tests...")
    add_subdirectory(test)
    message(STATUS "Building examples...")
    add_subdirectory(example)
endif()