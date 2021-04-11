if(NOT MIDDLEWARE_FATFS_USB_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_FATFS_USB_K32L2A41A_INCLUDED true CACHE BOOL "middleware_fatfs_usb component is included.")

    if(CONFIG_USE_middleware_baremetal_K32L2A41A)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk/fsl_usb_disk_bm.c
    )
    elseif(CONFIG_USE_middleware_freertos-kernel_K32L2A41A)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk/fsl_usb_disk_freertos.c
    )
    else()
        message(WARNING "please config middleware.baremetal_K32L2A41A or middleware.freertos-kernel_K32L2A41A first.")
    endif()


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk
    )


    include(middleware_fatfs_K32L2A41A)

    include(middleware_usb_host_stack_K32L2A41A)

    include(middleware_usb_host_msd_K32L2A41A)

endif()
