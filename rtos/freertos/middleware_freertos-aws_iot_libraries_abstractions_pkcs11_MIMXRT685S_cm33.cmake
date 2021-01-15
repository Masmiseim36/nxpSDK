if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_PKCS11_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_PKCS11_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_abstractions_pkcs11 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/pkcs11/mbedtls/iot_pkcs11_mbedtls.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/pkcs11/include
    )


    include(middleware_mbedtls_MIMXRT685S_cm33)

    include(middleware_freertos-kernel_MIMXRT685S_cm33)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT685S_cm33)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11_MIMXRT685S_cm33)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT685S_cm33)

    include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT685S_cm33)

    include(middleware_mbedtls_MIMXRT685S_cm33)

endif()
