set(SOURCES
    sensors.c
)

add_library(connfwk-Sensors ${SOURCES})

target_include_directories(connfwk-Sensors
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-Sensors
    PRIVATE
        connfwk-config
        ${CONNFWK_PLATFORM_LIB}
        ${CONNFWK_MCUX_SDK_LIB}
)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-Sensors-coverage ${SOURCES})
    target_link_libraries(connfwk-Sensors-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-Sensors-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-Sensors-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
