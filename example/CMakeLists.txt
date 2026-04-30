set(EXAMPLES 
    hello 
    optional 
    result 
    strings
    algebra
    render
    scalar
    vector
)

foreach(E IN LISTS EXAMPLES)
    add_executable(example_${E} ${E}.c)
    target_link_libraries(example_${E} PRIVATE ks::lib)
endforeach()