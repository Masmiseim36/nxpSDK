set(SOURCES
    Source/OtaSupport.c
    Source/OtaExternalFlash.c
    Source/OtaInternalFlash.c
)

add_library(connfwk-OtaSupport ${SOURCES})

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-OtaSupport PRIVATE connfwk-config)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-OtaSupport-coverage ${SOURCES})
    target_link_libraries(connfwk-OtaSupport-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-OtaSupport-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-OtaSupport-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
