set(LIST_INC "${CMAKE_SOURCE_DIR}/include/kitsune/data/list")

# LinkedList
kitsune_create_module(
    NAME LinkedList
    SOURCES linked_list.c
    HEADERS ${LIST_INC}/linked_list.h
)