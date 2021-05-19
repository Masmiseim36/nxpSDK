if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT1176_SOURCES_MIMXRT1176_cm4_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT1176_SOURCES_MIMXRT1176_cm4_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT1176_sources component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/bl_lpuart_irq_config_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/clock_cfg_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/external_memory_property_map_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/flexspi_config_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/flexspi_nand_config_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/hardware_init_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/memory_map_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/peripherals_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/sdmmc_config_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/sdmmc_convert_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/security_config_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/semc_nand_config_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/common/src/pinmux_utility_imxrt_series.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/sram_init_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/bootloader/src/lpuart_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/bootloader/src/usb_hid_msc_peripheral_interface.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/property/src/property_imx.c
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/puf/fsl_puf.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/usdhc/fsl_usdhc.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/security/keyblob/src/keyblob_caam.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/security/keyblob/src/keyblob.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/authentication/key_store_puf.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/memory/src/mmc_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/memory/src/sd_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/sdmmc/src/fsl_mmc.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/sdmmc/src/fsl_sd.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/sdmmc/src/fsl_sdmmc_common.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/sdmmc/port/usdhc/polling/fsl_sdmmc_host.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/memory/src/semc_nand_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src/semc_nand_flash_MIMXRT1176.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/nand_ecc/nand_ecc.c
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/semc/fsl_semc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/targets/MIMXRT1176/src
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/bootloader
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/include
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/startup
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/memory
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/memory/src
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/property
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/authentication
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/security/keyblob
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/flexspi_nor
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/flexspi
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/serial_nor_eeprom
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/sdmmc/inc
        ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/puf
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/semc
        ${CMAKE_CURRENT_LIST_DIR}/middleware/mcu-boot/src/drivers/nand_ecc
    )


endif()
