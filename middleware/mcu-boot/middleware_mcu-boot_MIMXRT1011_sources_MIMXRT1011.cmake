if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1011_SOURCES_MIMXRT1011_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1011_SOURCES_MIMXRT1011_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1011_sources component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/bl_clock_config_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/hardware_init_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/memory_map_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/external_memory_property_map_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/flexspi_nor_flash_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/bl_peripherals_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_imxrt_series.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/bl_lpuart_irq_config_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/sram_init_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/bl_dcp_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src/bl_romapi_MIMXRT1011.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpuart_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_nor_encrypt_otfad.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/usb_hid_msc_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/flexspi_nor_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/src/property/src/property_imx.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1011/src
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nor
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/smc
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dcp
        ${CMAKE_CURRENT_LIST_DIR}/src/drivers/trng
        ${CMAKE_CURRENT_LIST_DIR}/src/startup
    )


endif()
