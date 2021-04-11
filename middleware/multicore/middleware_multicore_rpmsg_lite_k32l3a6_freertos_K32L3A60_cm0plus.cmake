if(NOT MIDDLEWARE_MULTICORE_RPMSG_LITE_K32L3A6_FREERTOS_K32L3A60_cm0plus_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_RPMSG_LITE_K32L3A6_FREERTOS_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "middleware_multicore_rpmsg_lite_k32l3a6_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/porting/platform/k32l3a6/rpmsg_platform.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include/platform/k32l3a6
    )


    include(middleware_multicore_rpmsg_lite_K32L3A60_cm0plus)

    include(middleware_freertos-kernel_K32L3A60_cm0plus)

    include(middleware_freertos-kernel_heap_3_K32L3A60_cm0plus)

endif()
