set(SOURCES
    fwk_filesystem.c
)

add_library(connfwk-FileSystem ${SOURCES})

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-FileSystem PRIVATE connfwk-config)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-FileSystem-coverage ${SOURCES})
    target_link_libraries(connfwk-FileSystem-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-FileSystem-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-FileSystem-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
