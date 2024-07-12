string(TOUPPER ${CONNFWK_SECLIB} CONNFWK_SECLIB)

if(CONNFWK_SECLIB MATCHES "SW")
    set(SOURCES
        SecLib.c
        SecLib_aes_mmo.c
    )
elseif(CONNFWK_SECLIB MATCHES "SSS")
    set(SOURCES
        SecLib_sss.c
        SecLib_aes_mmo.c
    )
elseif(CONNFWK_SECLIB MATCHES "MBEDTLS")
    set(SOURCES
        SecLib_mbedTLS.c
        SecLib_aes_mmo.c
    )
else()
    message( SEND_ERROR "CONNFWK_SECLIB value not supported!" )
endif()

add_library(connfwk-SecLib ${SOURCES})

target_include_directories(connfwk-SecLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(connfwk-SecLib
    PUBLIC
        CONNFWK_SECLIB_${CONNFWK_SECLIB}
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-SecLib
    PRIVATE
        connfwk-config
        connfwk-FunctionLib
        connfwk-RNG
        ${CONNFWK_PLATFORM_LIB}
        ${CONNFWK_MCUX_SDK_LIB}
)

add_library(connfwk-SecLib-CL STATIC IMPORTED GLOBAL)
set_target_properties(connfwk-SecLib-CL PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib_crypto_${CONNFWK_LIB_CRYPTO_SUFFIX}.a)

target_link_libraries(connfwk-SecLib PRIVATE connfwk-SecLib-CL)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    message("lib_crypto" ${CONNFWK_LIB_CRYPTO_SUFFIX})
    add_library(connfwk-SecLib-coverage ${SOURCES})
    target_link_libraries(connfwk-SecLib-coverage
        PRIVATE
            connfwk-config
            connfwk-FunctionLib
            connfwk-RNG
            ${CONNFWK_PLATFORM_LIB}
            ${CONNFWK_MCUX_SDK_LIB}
    )

    target_link_libraries(connfwk-SecLib-Coverage PRIVATE connfwk-SecLib-CL)

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

