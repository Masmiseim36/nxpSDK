if(NOT MIDDLEWARE_MCU-BOOT_DRV_FLEXSPI_NAND_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_DRV_FLEXSPI_NAND_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_drv_flexspi_nand component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi/bl_flexspi.c
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nand/flexspi_nand_flash.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nand
    )


endif()
