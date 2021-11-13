include_guard(GLOBAL)
message("middleware_mcu-boot_MKE16Z4_sources component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MKE16Z4/src/bl_clock_config_MKE16Z4.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MKE16Z4/src/hardware_init_MKE16Z4.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MKE16Z4/src/memory_map_MKE16Z4.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MKE16Z4/src/bl_peripherals_MKE16Z4.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_common.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/bl_lpi2c_irq_config_common.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/bl_lpuart_irq_config_common.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/bl_lpspi_irq_config_common.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpuart_peripheral_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpspi_peripheral_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpi2c_peripheral_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/mscan_peripheral_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/sram_init_cm0plus.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MKE16Z4/src
)


