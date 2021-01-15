if(NOT MIDDLEWARE_MCU-BOOT_DRV_SERIAL_NOR_EEPROM_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_DRV_SERIAL_NOR_EEPROM_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_mcu-boot_drv_serial_nor_eeprom component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/serial_nor_eeprom/serial_nor_eeprom.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/serial_nor_eeprom
    )


    include(driver_lpspi_MIMXRT1064)

endif()
