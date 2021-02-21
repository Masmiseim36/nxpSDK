if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_SECURE_SOCKETS_LWIP_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_SECURE_SOCKETS_LWIP_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/secure_sockets/lwip/iot_secure_sockets.c
    )


    include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1176_cm7)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT1176_cm7)

    include(middleware_lwip_MIMXRT1176_cm7)

    include(middleware_freertos-kernel_MIMXRT1176_cm7)

endif()
