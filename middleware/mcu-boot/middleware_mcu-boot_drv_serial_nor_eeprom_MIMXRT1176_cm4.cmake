include_guard(GLOBAL)
message("middleware_mcu-boot_drv_serial_nor_eeprom component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/serial_nor_eeprom/serial_nor_eeprom.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/serial_nor_eeprom
)


include(driver_lpspi_MIMXRT1176_cm4)

