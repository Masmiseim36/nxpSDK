string(TOUPPER ${CONNFWK_RNG} CONNFWK_RNG)

if(CONNFWK_RNG MATCHES "SW")
    set(SOURCES
        RNG.c
    )
elseif(CONNFWK_RNG MATCHES "MBEDTLS")
    set(SOURCES
        RNG_mbedTLS.c
    )
else()
    message( SEND_ERROR "CONNFWK_RNG value not supported!" )
endif()

add_library(connfwk-RNG ${SOURCES})

target_include_directories(connfwk-RNG
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-RNG
    PRIVATE
        connfwk-config
        connfwk-FunctionLib
        connfwk-SecLib
        ${CONNFWK_MCUX_SDK_LIB}
)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-RNG-coverage ${SOURCES})
    target_link_libraries(connfwk-RNG-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-RNG-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-RNG-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()

