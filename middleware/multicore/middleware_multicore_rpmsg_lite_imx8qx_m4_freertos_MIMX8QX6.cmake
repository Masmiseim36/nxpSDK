if(NOT MIDDLEWARE_MULTICORE_RPMSG_LITE_IMX8QX_M4_FREERTOS_MIMX8QX6_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_RPMSG_LITE_IMX8QX_M4_FREERTOS_MIMX8QX6_INCLUDED true CACHE BOOL "middleware_multicore_rpmsg_lite_imx8qx_m4_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/porting/platform/imx8qx_cm4/rpmsg_platform.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include/platform/imx8qx_cm4
    )


    include(middleware_multicore_rpmsg_lite_MIMX8QX6)

    include(middleware_freertos-kernel_MIMX8QX6)

    include(middleware_freertos-kernel_heap_4_MIMX8QX6)

endif()
