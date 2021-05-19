if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_SECURE_SOCKETS_LWIP_MIMXRT1166_cm4_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_VENDOR_NXP_SECURE_SOCKETS_LWIP_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/secure_sockets/lwip/iot_secure_sockets.c
    )


    include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1166_cm4)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT1166_cm4)

    include(middleware_lwip_MIMXRT1166_cm4)

    include(middleware_freertos-kernel_MIMXRT1166_cm4)

endif()
