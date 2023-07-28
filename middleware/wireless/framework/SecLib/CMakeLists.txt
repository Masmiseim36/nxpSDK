string(TOUPPER ${CONNFWK_SECLIB} CONNFWK_SECLIB)

set(SOURCES_CRYPTOLIB
    build/CryptoLib/src/big_integer.c
    build/CryptoLib/src/ec_p_256_with_macros.c
    build/CryptoLib/src/ec_p_256.c
    build/CryptoLib/src/ecdh.c
    build/CryptoLib/src/SW_AES128.c
    build/CryptoLib/src/SW_RNG.c
    build/CryptoLib/src/SW_SHA1.c
    build/CryptoLib/src/SW_SHA256.c
)

add_library(connfwk-SecLib-CryptoLib ${SOURCES_CRYPTOLIB})

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-SecLib-CryptoLib PRIVATE connfwk-config)

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

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-SecLib-CryptoLib-coverage ${SOURCES})
    target_link_libraries(connfwk-SecLib-CryptoLib-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-SecLib-CryptoLib-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-SecLib-CryptoLib-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()

add_library(connfwk-SecLib ${SOURCES})

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-SecLib
    PUBLIC
        connfwk-SecLib-CryptoLib
    PRIVATE
        connfwk-config
)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-SecLib-coverage ${SOURCES})
    target_link_libraries(connfwk-SecLib-coverage
        PUBLIC
            connfwk-SecLib-CryptoLib
        PRIVATE
            connfwk-config
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

