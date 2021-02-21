if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_CRYPTO_MIMXRT1176_cm4_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_CRYPTO_MIMXRT1176_cm4_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/crypto/src/iot_crypto.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/crypto/include
    )


    include(middleware_freertos-aws_iot_libraries_abstractions_threading_alt_MIMXRT1176_cm4)

    include(middleware_mbedtls_MIMXRT1176_cm4)

endif()
