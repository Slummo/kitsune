function(kitsune_create_module)
    set(options INTERFACE)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES HEADERS DEPS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(LIB_NAME "kitsune_${ARG_NAME}")
    set(ALIAS_NAME "Kitsune::${ARG_NAME}")

    if(ARG_INTERFACE)
        add_library(${LIB_NAME} INTERFACE)

        if(ARG_HEADERS)
            target_sources(${LIB_NAME} INTERFACE ${ARG_HEADERS})
        endif()

        target_include_directories(${LIB_NAME} INTERFACE
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        )

        if(ARG_DEPS)
            target_link_libraries(${LIB_NAME} INTERFACE ${ARG_DEPS})
        endif()
    else()
        add_library(${LIB_NAME} STATIC ${ARG_SOURCES} ${ARG_HEADERS})

        target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        )

        if(ARG_DEPS)
            target_link_libraries(${LIB_NAME} PUBLIC ${ARG_DEPS})
        endif()
    endif()

    add_library(${ALIAS_NAME} ALIAS ${LIB_NAME})
endfunction()

function(kitsune_add_test)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES DEPS)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(TEST_NAME "test_${ARG_NAME}")

    add_executable(${TEST_NAME} ${ARG_SOURCES})
    target_link_libraries(${TEST_NAME} PRIVATE
        unity
        ${ARG_DEPS}
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()