add_executable(hello_world 1-hello_world/main.c)
target_link_libraries(hello_world PRIVATE
    Kitsune::Core
)

add_executable(optional 2-optional/main.c)
target_link_libraries(optional PRIVATE
    Kitsune::Core
)