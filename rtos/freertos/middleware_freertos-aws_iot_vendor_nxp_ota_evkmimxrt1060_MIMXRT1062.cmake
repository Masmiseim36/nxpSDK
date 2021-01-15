if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_OTA_EVKMIMXRT1060_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_OTA_EVKMIMXRT1060_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_vendor_nxp_ota_evkmimxrt1060 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/ota/evkmimxrt1060/aws_ota_pal.c
    )


    include(middleware_freertos-aws_iot_libraries_freertos_plus_aws_ota_MIMXRT1062)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1062)

    include(component_mflash_file_MIMXRT1062)

    include(middleware_freertos-aws_iot_demos_MIMXRT1062)

    include(driver_common_MIMXRT1062)

endif()
