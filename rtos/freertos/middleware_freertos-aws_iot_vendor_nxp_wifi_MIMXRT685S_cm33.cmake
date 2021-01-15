if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_WIFI_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_WIFI_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_vendor_nxp_wifi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/wifi/nxp/iot_wifi.c
    )


    include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT685S_cm33)

    include(middleware_wifi_MIMXRT685S_cm33)

endif()
