if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_TLS_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_TLS_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/tls/src/iot_tls.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/tls/include
    )


    include(middleware_freertos-aws_iot_common_MIMXRT1052)

    include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT1052)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1052)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11_MIMXRT1052)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT1052)

    include(middleware_freertos-kernel_MIMXRT1052)

    include(middleware_mbedtls_MIMXRT1052)

endif()
