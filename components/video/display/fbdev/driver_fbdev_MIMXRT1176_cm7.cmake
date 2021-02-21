if(NOT DRIVER_FBDEV_MIMXRT1176_cm7_INCLUDED)
    
    set(DRIVER_FBDEV_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "driver_fbdev component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_fbdev.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dc-fb-common_MIMXRT1176_cm7)

    include(middleware_freertos-kernel_MIMXRT1176_cm7)

endif()
