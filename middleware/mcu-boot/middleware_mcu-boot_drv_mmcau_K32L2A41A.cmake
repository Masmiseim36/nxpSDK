if(NOT MIDDLEWARE_MCU-BOOT_DRV_MMCAU_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_DRV_MMCAU_K32L2A41A_INCLUDED true CACHE BOOL "middleware_mcu-boot_drv_mmcau component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mmcau/src/mmcau_aes_functions.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mmcau
    )


endif()
