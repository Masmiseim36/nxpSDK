string(TOUPPER ${CONNFWK_SECLIB} CONNFWK_SECLIB)

if(CONNFWK_SECLIB MATCHES "SW")
    set(SOURCES
        SecLib.c
    )
elseif(CONNFWK_SECLIB MATCHES "SSS")
    set(SOURCES
        SecLib_sss.c
    )
elseif(CONNFWK_SECLIB MATCHES "MBEDTLS")
    set(SOURCES
        SecLib_mbedTLS.c
    )
else()
    message( SEND_ERROR "CONNFWK_SECLIB value not supported!" )
endif()

add_library(connfwk-SecLib ${SOURCES})

target_include_directories(connfwk-SecLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-SecLib
    PUBLIC
    PRIVATE
        connfwk-config
        connfwk-FunctionLib
        connfwk-RNG
        ${CMAKE_CURRENT_SOURCE_DIR}/lib_crypto_${CONNFWK_LIB_CRYPTO_SUFFIX}.a
        ${CONNFWK_PLATFORM_LIB}
        ${CONNFWK_MCUX_SDK_LIB}
)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-SecLib-coverage ${SOURCES})
    target_link_libraries(connfwk-SecLib-coverage
        PUBLIC
        PRIVATE
            connfwk-config
            ${CMAKE_CURRENT_SOURCE_DIR}/lib_crypto_${CONNFWK_LIB_CRYPTO_SUFFIX}.a
    )

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-SecLib-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-SecLib-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
