include_guard()
message("middleware_mcu-boot_drv_flexspi_nor component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi/bl_flexspi.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nor/flexspi_nor_flash.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nor
)


