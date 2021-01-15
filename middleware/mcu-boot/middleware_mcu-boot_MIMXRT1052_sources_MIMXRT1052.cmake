if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1052_SOURCES_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1052_SOURCES_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1052_sources component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/bl_clock_config_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/hardware_init_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/memory_map_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/external_memory_property_map_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/flexspi_nand_config_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/flexspi_nor_config_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/semc_nand_config_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/semc_nor_config_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/bl_peripherals_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_imxrt_series.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/bl_lpuart_irq_config_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/sdmmc_config_MIMXRT1052.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src/sram_init_MIMXRT1052.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1052/src
    )


endif()
