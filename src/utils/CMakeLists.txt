set(UTILS_INC "${CMAKE_SOURCE_DIR}/include/kitsune/utils")

# Bit
kitsune_create_module(
    NAME Bit
    INTERFACE
    HEADERS ${UTILS_INC}/bit.h
)