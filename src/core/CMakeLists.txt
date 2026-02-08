# Core
set(CORE_INC "${CMAKE_SOURCE_DIR}/include/kitsune/core")
kitsune_create_module(
    NAME Core
    INTERFACE
    HEADERS ${CORE_INC}/log.h
)