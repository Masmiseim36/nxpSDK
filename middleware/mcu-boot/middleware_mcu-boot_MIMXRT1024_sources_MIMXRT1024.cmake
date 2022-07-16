include_guard()
message("middleware_mcu-boot_MIMXRT1024_sources component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/bl_api.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/bl_clock_config_MIMXRT1024.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/hardware_init_MIMXRT1024.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/memory_map_MIMXRT1024.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/external_memory_property_map_MIMXRT1024.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/flexspi_nor_config_MIMXRT1024.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/bl_peripherals_MIMXRT1024.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_imxrt_series.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/bl_lpuart_irq_config_MIMXRT1024.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src/sram_init_MIMXRT1024.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1024/src
)


