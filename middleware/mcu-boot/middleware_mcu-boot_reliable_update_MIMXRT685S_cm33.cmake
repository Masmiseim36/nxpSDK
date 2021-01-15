if(NOT MIDDLEWARE_MCU-BOOT_RELIABLE_UPDATE_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_RELIABLE_UPDATE_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_mcu-boot_reliable_update component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_reliable_update_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
    )


endif()
