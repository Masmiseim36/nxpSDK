if(NOT MIDDLEWARE_MULTICORE_RPMSG_LITE_IMX7ULP_M4_FREERTOS_MCIMX7U5_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_RPMSG_LITE_IMX7ULP_M4_FREERTOS_MCIMX7U5_INCLUDED true CACHE BOOL "middleware_multicore_rpmsg_lite_imx7ulp_m4_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/porting/platform/imx7ulp_m4/rpmsg_platform.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include/platform/imx7ulp_m4
    )


    include(middleware_multicore_rpmsg_lite_MCIMX7U5)

    include(middleware_freertos-kernel_MCIMX7U5)

    include(middleware_freertos-kernel_heap_4_MCIMX7U5)

endif()
