include_guard(GLOBAL)
message("middleware_mcu-boot_MIMXRT1176_sources component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/bl_lpuart_irq_config_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/clock_cfg_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/external_memory_property_map_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/flexspi_config_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/flexspi_nand_config_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/hardware_init_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/memory_map_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/periph_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/sdmmc_config_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/sdmmc_convert_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/security_config_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/semc_nand_config_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_imxrt_series.c
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src/sram_init_MIMXRT1176.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpuart_peripheral_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/usb_hid_msc_peripheral_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/src/property/src/property_imx.c
    ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/puf/fsl_puf.c
    ${CMAKE_CURRENT_LIST_DIR}/src/security/keyblob/src/keyblob_caam.c
    ${CMAKE_CURRENT_LIST_DIR}/src/security/keyblob/src/keyblob.c
    ${CMAKE_CURRENT_LIST_DIR}/src/authentication/key_store_puf.c
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/semc_nand_memory.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/nand_ecc/nand_ecc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc/bl_semc.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1176/src
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
    ${CMAKE_CURRENT_LIST_DIR}/src/include
    ${CMAKE_CURRENT_LIST_DIR}/src/startup
    ${CMAKE_CURRENT_LIST_DIR}/src/memory
    ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
    ${CMAKE_CURRENT_LIST_DIR}/src/property
    ${CMAKE_CURRENT_LIST_DIR}/src/authentication
    ${CMAKE_CURRENT_LIST_DIR}/src/security/keyblob
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nor
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/serial_nor_eeprom
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/inc
    ${CMAKE_CURRENT_LIST_DIR}/../../platform/drivers/puf
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/nand_ecc
)


