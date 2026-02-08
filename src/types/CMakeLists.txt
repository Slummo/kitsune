# Number
set(NUMBER_INC "${CMAKE_SOURCE_DIR}/include/kitsune/types/number")
kitsune_create_module(
    NAME Number
    INTERFACE
    HEADERS ${NUMBER_INC}/integer.h ${NUMBER_INC}/float.h
)

# String
set(STRING_INC "${CMAKE_SOURCE_DIR}/include/kitsune/types/string")
kitsune_create_module(
    NAME String
    SOURCES string/string.c string/str_view.c
    HEADERS ${STRING_INC}/string.h ${STRING_INC}/str_view.h
    DEPS Kitsune::Number
)