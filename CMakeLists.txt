cmake_minimum_required(VERSION 4.0)
project(kitsune VERSION 1.0.0 LANGUAGES C)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/common.cmake)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_SELF TRUE)
else()
    set(IS_SELF FALSE)
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

if(IS_SELF)
    set_default_build_type()
    set_output_directories()
endif()

# Library
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::lib ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE c_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

if (IS_SELF)
    apply_compile_options(${PROJECT_NAME})
endif()

# Dependencies
find_package(OpenMP REQUIRED)
find_package(BLAS REQUIRED)
find_package(sleef CONFIG REQUIRED)

find_path(CBLAS_INCLUDE_DIR 
    NAMES cblas.h
    PATH_SUFFIXES openblas flexiblas
)

if(NOT CBLAS_INCLUDE_DIR)
    message(FATAL_ERROR "cblas.h not found! Please install the BLAS development headers.")
endif()

target_link_libraries(${PROJECT_NAME} INTERFACE
    m
    OpenMP::OpenMP_C
    BLAS::BLAS
    sleef
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CBLAS_INCLUDE_DIR}
)

# Tests & Examples
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
if (IS_SELF AND CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
    message(STATUS "Building tests...")
    add_subdirectory(test)
    message(STATUS "Building examples...")
    add_subdirectory(example)
endif()