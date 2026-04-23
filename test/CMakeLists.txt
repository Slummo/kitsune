include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY  https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG         v2.6.1
)
FetchContent_MakeAvailable(unity)

set(TESTS 
    strings
)

foreach(T IN LISTS TESTS)
    add_executable(test_${T} ${T}.c)
    target_link_libraries(test_${T} PRIVATE kitsune unity)
endforeach()