if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_PKCS11_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_PKCS11_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/pkcs11/src/iot_pkcs11.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/pkcs11/include
    )


    include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MIMXRT685S_cm33)

    include(middleware_freertos-kernel_MIMXRT685S_cm33)

endif()
