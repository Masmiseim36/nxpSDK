if(NOT MIDDLEWARE_MCU-BOOT_DRV_SDMMC_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_DRV_SDMMC_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_drv_sdmmc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/usdhc/bl_usdhc.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/port/usdhc/polling/bl_host.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/bl_sd.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/bl_mmc.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/src/bl_sdmmc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/usdhc
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/inc
    )


endif()
