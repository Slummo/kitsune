enable_testing()

include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY  https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG         v2.6.1
)
FetchContent_MakeAvailable(unity)

kitsune_add_test(
    NAME strings
    SOURCES core/strings.c
    DEPS Kitsune::Core
)

# LinkedList
set(TEST_LIST_ROOT "data/list")
kitsune_add_test(
    NAME linked_list
    SOURCES ${TEST_LIST_ROOT}/linked_list.c
    DEPS Kitsune::LinkedList
)

