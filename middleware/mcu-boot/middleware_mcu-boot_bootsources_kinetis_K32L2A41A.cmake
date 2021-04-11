if(NOT MIDDLEWARE_MCU-BOOT_BOOTSOURCES_KINETIS_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_BOOTSOURCES_KINETIS_K32L2A41A_INCLUDED true CACHE BOOL "middleware_mcu-boot_bootsources_kinetis component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/smc/smc.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/flash_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/src/property/src/property_kinetis.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_reliable_update.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/smc
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
        ${CMAKE_CURRENT_LIST_DIR}/src/utilities
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
    )


endif()
