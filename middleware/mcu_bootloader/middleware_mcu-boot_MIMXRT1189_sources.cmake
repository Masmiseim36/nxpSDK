# Add set(CONFIG_USE_middleware_mcu-boot_MIMXRT1189_sources true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/bl_lpuart_irq_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/bl_lpspi_irq_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/clock_cfg_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/external_memory_property_map_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/flexspi_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/flexspi_nand_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/flexspi_nor_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/hardware_init_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/lpspi_mem_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/memory_map_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/periph_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/sdmmc_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/sdmmc_convert_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/semc_nand_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/edgelock_config_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/common/src/pinmux_utility_imxrt_series.c
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src/sram_init_MIMXRT1189.c
          ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpuart_peripheral_interface.c
          ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/lpspi_peripheral_interface.c
          ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/usb_hid_msc_peripheral_interface.c
          ${CMAKE_CURRENT_LIST_DIR}/src/property/src/property_imx.c
          ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/semc_nand_memory.c
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/nand_ecc/nand_ecc.c
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc/bl_semc.c
          ${CMAKE_CURRENT_LIST_DIR}/src/security/keyblob/src/keyblob_edgelock.c
          ${CMAKE_CURRENT_LIST_DIR}/src/security/keyblob/src/keyblob.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1189/src
          ${CMAKE_CURRENT_LIST_DIR}/src/security/keyblob
          ${CMAKE_CURRENT_LIST_DIR}/src
          ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
          ${CMAKE_CURRENT_LIST_DIR}/src/include
          ${CMAKE_CURRENT_LIST_DIR}/src/startup
          ${CMAKE_CURRENT_LIST_DIR}/src/memory
          ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
          ${CMAKE_CURRENT_LIST_DIR}/src/property
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi_nor
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/flexspi
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/serial_nor_eeprom
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/sdmmc/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/semc
          ${CMAKE_CURRENT_LIST_DIR}/src/drivers/nand_ecc
        )

  
