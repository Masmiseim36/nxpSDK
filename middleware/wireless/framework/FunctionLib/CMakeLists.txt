set(SOURCES
    FunctionLib.c
)

add_library(connfwk-FunctionLib ${SOURCES})

target_include_directories(connfwk-FunctionLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-FunctionLib PRIVATE connfwk-config)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-FunctionLib-coverage ${SOURCES})
    target_link_libraries(connfwk-FunctionLib-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-FunctionLib-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-FunctionLib-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
