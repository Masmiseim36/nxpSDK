include_guard(GLOBAL)
message("middleware_mcu-boot_drv_flexspi_nand component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi/bl_flexspi.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nand/flexspi_nand_flash.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nand
)


