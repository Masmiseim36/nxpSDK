include_guard(GLOBAL)
message("middleware_mcu-boot_mem_spi_nor_eeprom component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/spi_nor_eeprom_memory.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
)


include(middleware_mcu-boot_drv_serial_nor_eeprom_MIMXRT1176_cm4)

