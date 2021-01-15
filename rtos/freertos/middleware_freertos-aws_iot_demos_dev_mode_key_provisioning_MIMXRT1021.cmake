if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_DEV_MODE_KEY_PROVISIONING_MIMXRT1021_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_DEV_MODE_KEY_PROVISIONING_MIMXRT1021_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_demos_dev_mode_key_provisioning component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/dev_mode_key_provisioning/src/aws_dev_mode_key_provisioning.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/dev_mode_key_provisioning/include
    )


    include(middleware_freertos-aws_iot_common_MIMXRT1021)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11_MIMXRT1021)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT1021)

    include(middleware_freertos-kernel_MIMXRT1021)

    include(middleware_mbedtls_MIMXRT1021)

endif()
