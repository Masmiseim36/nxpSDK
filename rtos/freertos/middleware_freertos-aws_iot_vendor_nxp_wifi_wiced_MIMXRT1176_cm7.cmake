if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_WIFI_WICED_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_WIFI_WICED_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_vendor_nxp_wifi_wiced component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/wifi/wiced/iot_wifi.c
    )


    include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT1176_cm7)

    include(middleware_wifi_wiced_MIMXRT1176_cm7)

    include(middleware_lwip_MIMXRT1176_cm7)

    include(middleware_freertos-kernel_MIMXRT1176_cm7)

endif()
