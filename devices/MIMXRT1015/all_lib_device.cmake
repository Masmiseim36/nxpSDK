# Copy variable into project config.cmake to use software component
#set.board.evkmimxrt1015
#  # description: XIP Board Driver
#  set(CONFIG_USE_board_boot_header true)

#  # description: Board_project_template evkmimxrt1015
#  set(CONFIG_USE_board_project_template true)

#set.device.MIMXRT1015
#  # description: Device Boot Header
#  set(CONFIG_USE_device_boot_header true)

#  # description: Clock Driver
#  set(CONFIG_USE_driver_clock true)

#  # description: Rte_device
#  set(CONFIG_USE_device_RTE true)

#  # description: Utility notifier
#  set(CONFIG_USE_utility_notifier true)

#  # description: Utilities which is needed for particular toolchain like the SBRK function required to address limitation between HEAP and STACK in GCC toolchain library.
#  set(CONFIG_USE_utilities_misc_utilities true)

#  # description: Used to include slave core binary into master core binary.
#  set(CONFIG_USE_utility_incbin true)

#  # description: Utility debug_console Template Config
#  set(CONFIG_USE_utility_debug_console_template_config true)

#  # description: common Driver
#  set(CONFIG_USE_driver_common true)

#  # description: Driver ektf2k
#  set(CONFIG_USE_driver_ektf2k true)

#  # description: Touch panel controller FT3267 driver
#  set(CONFIG_USE_driver_ft3267 true)

#  # description: Driver ft5406
#  set(CONFIG_USE_driver_ft5406 true)

#  # description: Driver gt911
#  set(CONFIG_USE_driver_gt911 true)

#  # description: Driver tma525b
#  set(CONFIG_USE_driver_tma525b true)

#  # description: Component serial_manager_swo
#  set(CONFIG_USE_component_serial_manager_swo true)

#  # description: Component serial_manager_virtual
#  set(CONFIG_USE_component_serial_manager_virtual true)

#  # description: RTT template configuration
#  set(CONFIG_USE_driver_rtt_template true)

#  # description: Component software_rng_adapter
#  set(CONFIG_USE_component_software_rng_adapter true)

#  # description: Component reset_adapter
#  set(CONFIG_USE_component_reset_adapter true)

#  # description: Component panic
#  set(CONFIG_USE_component_panic true)

#  # description: Driver mx25r_flash
#  set(CONFIG_USE_driver_mx25r_flash true)

#  # description: Component software_crc_adapter
#  set(CONFIG_USE_component_software_crc_adapter true)

#  # description: Component ak4497 adapter for single codec
#  set(CONFIG_USE_component_ak4497_adapter true)

#  # description: Component codec adapters for multi codec
#  set(CONFIG_USE_component_codec_adapters true)

#  # description: Component cs42448 adapter for single codec
#  set(CONFIG_USE_component_cs42448_adapter true)

#  # description: Component cs42888 adapter for single codec
#  set(CONFIG_USE_component_cs42888_adapter true)

#  # description: Component da7212 adapter for single codec
#  set(CONFIG_USE_component_da7212_adapter true)

#  # description: Component pcm186x adapter for single codec
#  set(CONFIG_USE_component_pcm186x_adapter true)

#  # description: Component pcm512x adapter for single codec
#  set(CONFIG_USE_component_pcm512x_adapter true)

#  # description: Component sgtl5000 adapter for single codec
#  set(CONFIG_USE_component_sgtl_adapter true)

#  # description: Component tfa9896 adapter for single codec
#  set(CONFIG_USE_component_tfa9896_adapter true)

#  # description: Component tfa9xxx adapter for single codec
#  set(CONFIG_USE_component_tfa9xxx_adapter true)

#  # description: Component wm8524 adapter for single codec
#  set(CONFIG_USE_component_wm8524_adapter true)

#  # description: Component wm8904 adapter for single codec
#  set(CONFIG_USE_component_wm8904_adapter true)

#  # description: Component wm8960 adapter for single codecs
#  set(CONFIG_USE_component_wm8960_adapter true)

#  # description: Component wm8962 adapter for single codecs
#  set(CONFIG_USE_component_wm8962_adapter true)

#  # description: Driver codec
#  set(CONFIG_USE_driver_codec true)

#  # description: Devices_project_template MIMXRT1015
#  set(CONFIG_USE_device_project_template true)

#  # description: SOC FLEXRAM ALLOCATE Driver
#  set(CONFIG_USE_driver_flexram_allocate true)

#  # description: IOMUXC Driver
#  set(CONFIG_USE_driver_iomuxc true)

#  # description: NIC301 Driver
#  set(CONFIG_USE_driver_nic301 true)

#  # description: ROMAPI Driver
#  set(CONFIG_USE_driver_romapi true)

#  # description: Device MIMXRT1015_cmsis
#  set(CONFIG_USE_device_CMSIS true)

#  # description: Device MIMXRT1015_system
#  set(CONFIG_USE_device_system true)

#  # description: Device MIMXRT1015_startup
#  set(CONFIG_USE_device_startup true)

#  # description: Utility str
#  set(CONFIG_USE_utility_str true)

#  # description: Utility debug_console_lite
#  set(CONFIG_USE_utility_debug_console_lite true)

#  # description: Utility debug_console
#  set(CONFIG_USE_utility_debug_console true)

#  # description: Utility assert
#  set(CONFIG_USE_utility_assert true)

#  # description: Utility assert_lite
#  set(CONFIG_USE_utility_assert_lite true)

#  # description: LPUART Freertos Driver
#  set(CONFIG_USE_driver_lpuart_freertos true)

#  # description: LPSPI FreeRTOS Driver
#  set(CONFIG_USE_driver_lpspi_freertos true)

#  # description: LPI2C FreeRTOS Driver
#  set(CONFIG_USE_driver_lpi2c_freertos true)

#  # description: XBARB Driver
#  set(CONFIG_USE_driver_xbarb true)

#  # description: XBARA Driver
#  set(CONFIG_USE_driver_xbara true)

#  # description: wdog01 Driver
#  set(CONFIG_USE_driver_wdog01 true)

#  # description: TRNG Driver
#  set(CONFIG_USE_driver_trng true)

#  # description: TEMPMON Driver
#  set(CONFIG_USE_driver_tempmon true)

#  # description: SRC Driver
#  set(CONFIG_USE_driver_src true)

#  # description: SPDIF Driver
#  set(CONFIG_USE_driver_spdif true)

#  # description: SPDIF EDMA Driver
#  set(CONFIG_USE_driver_spdif_edma true)

#  # description: SNVS LP Driver
#  set(CONFIG_USE_driver_snvs_lp true)

#  # description: SNVS HP Driver
#  set(CONFIG_USE_driver_snvs_hp true)

#  # description: SAI Driver
#  set(CONFIG_USE_driver_sai true)

#  # description: SAI EDMA Driver
#  set(CONFIG_USE_driver_sai_edma true)

#  # description: RTWDOG Driver
#  set(CONFIG_USE_driver_rtwdog true)

#  # description: QTMR Driver
#  set(CONFIG_USE_driver_qtmr_1 true)

#  # description: PWM Driver
#  set(CONFIG_USE_driver_pwm true)

#  # description: pmu Driver
#  set(CONFIG_USE_driver_pmu true)

#  # description: PIT Driver
#  set(CONFIG_USE_driver_pit true)

#  # description: OCOTP Driver
#  set(CONFIG_USE_driver_ocotp true)

#  # description: LPUART Driver
#  set(CONFIG_USE_driver_lpuart true)

#  # description: LPUART Driver
#  set(CONFIG_USE_driver_lpuart_edma true)

#  # description: LPUART CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpuart true)

#  # description: LPSPI Driver
#  set(CONFIG_USE_driver_lpspi true)

#  # description: LPSPI Driver
#  set(CONFIG_USE_driver_lpspi_edma true)

#  # description: LPSPI CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpspi true)

#  # description: LPI2C Driver
#  set(CONFIG_USE_driver_lpi2c true)

#  # description: LPI2C Driver
#  set(CONFIG_USE_driver_lpi2c_edma true)

#  # description: LPI2C CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpi2c true)

#  # description: KPP Driver
#  set(CONFIG_USE_driver_kpp true)

#  # description: GPIO Driver
#  set(CONFIG_USE_driver_igpio true)

#  # description: GPT Driver
#  set(CONFIG_USE_driver_gpt true)

#  # description: GPC Driver
#  set(CONFIG_USE_driver_gpc_1 true)

#  # description: FLEXSPI Driver
#  set(CONFIG_USE_driver_flexspi true)

#  # description: FLEXSPI Driver
#  set(CONFIG_USE_driver_flexspi_edma true)

#  # description: FLEXRAM Driver
#  set(CONFIG_USE_driver_flexram true)

#  # description: FLEXIO UART Driver
#  set(CONFIG_USE_driver_flexio_uart true)

#  # description: FLEXIO UART EDMA Driver
#  set(CONFIG_USE_driver_flexio_uart_edma true)

#  # description: FLEXIO SPI Driver
#  set(CONFIG_USE_driver_flexio_spi true)

#  # description: FLEXIO SPI EDMA Driver
#  set(CONFIG_USE_driver_flexio_spi_edma true)

#  # description: FLEXIO MCULCD Driver
#  set(CONFIG_USE_driver_flexio_mculcd true)

#  # description: FLEXIO MCULCD EDMA Driver
#  set(CONFIG_USE_driver_flexio_mculcd_edma true)

#  # description: FLEXIO I2S Driver
#  set(CONFIG_USE_driver_flexio_i2s true)

#  # description: FLEXIO I2S EDMA Driver
#  set(CONFIG_USE_driver_flexio_i2s_edma true)

#  # description: FLEXIO I2C Driver
#  set(CONFIG_USE_driver_flexio_i2c_master true)

#  # description: FLEXIO Driver
#  set(CONFIG_USE_driver_flexio true)

#  # description: FLEXIO Driver
#  set(CONFIG_USE_driver_flexio_camera true)

#  # description: FLEXIO CAMERA EDMA Driver
#  set(CONFIG_USE_driver_flexio_camera_edma true)

#  # description: EWM Driver
#  set(CONFIG_USE_driver_ewm true)

#  # description: ENC Driver
#  set(CONFIG_USE_driver_enc true)

#  # description: EDMA Driver
#  set(CONFIG_USE_driver_edma true)

#  # description: DMAMUX Driver
#  set(CONFIG_USE_driver_dmamux true)

#  # description: DCP Driver
#  set(CONFIG_USE_driver_dcp true)

#  # description: DCDC Driver
#  set(CONFIG_USE_driver_dcdc_1 true)

#  # description: CACHE Driver
#  set(CONFIG_USE_driver_cache_armv7_m7 true)

#  # description: BEE Driver
#  set(CONFIG_USE_driver_bee true)

#  # description: AOI Driver
#  set(CONFIG_USE_driver_aoi true)

#  # description: aipstz Driver
#  set(CONFIG_USE_driver_aipstz true)

#  # description: ADC_ETC Driver
#  set(CONFIG_USE_driver_adc_etc true)

#  # description: ADC Driver
#  set(CONFIG_USE_driver_adc_12b1msps_sar true)

#  # description: Driver camera-common
#  set(CONFIG_USE_driver_camera-common true)

#  # description: Driver camera-device-ap1302
#  set(CONFIG_USE_driver_camera-device-ap1302 true)

#  # description: Driver camera-device-common
#  set(CONFIG_USE_driver_camera-device-common true)

#  # description: Driver camera-device-max9286
#  set(CONFIG_USE_driver_camera-device-max9286 true)

#  # description: Driver camera-device-mt9m114
#  set(CONFIG_USE_driver_camera-device-mt9m114 true)

#  # description: Driver camera-device-ov5640
#  set(CONFIG_USE_driver_camera-device-ov5640 true)

#  # description: Driver camera-device-ov7670
#  set(CONFIG_USE_driver_camera-device-ov7670 true)

#  # description: Driver camera-device-ov7725
#  set(CONFIG_USE_driver_camera-device-ov7725 true)

#  # description: Driver camera-device-sccb
#  set(CONFIG_USE_driver_camera-device-sccb true)

#  # description: Driver camera-receiver-common
#  set(CONFIG_USE_driver_camera-receiver-common true)

#  # description: Driver camera-receiver-flexio
#  set(CONFIG_USE_driver_camera-receiver-flexio true)

#  # description: Driver dbi
#  set(CONFIG_USE_driver_dbi true)

#  # description: Driver dbi_flexio_edma
#  set(CONFIG_USE_driver_dbi_flexio_edma true)

#  # description: Driver dc-fb-common
#  set(CONFIG_USE_driver_dc-fb-common true)

#  # description: Driver dc-fb-dbi
#  set(CONFIG_USE_driver_dc-fb-dbi true)

#  # description: Driver dc-fb-ssd1963
#  set(CONFIG_USE_driver_dc-fb-ssd1963 true)

#  # description: Driver display-adv7535
#  set(CONFIG_USE_driver_display-adv7535 true)

#  # description: Driver display-sn65dsi83
#  set(CONFIG_USE_driver_display-sn65dsi83 true)

#  # description: Driver display-common
#  set(CONFIG_USE_driver_display-common true)

#  # description: Driver display-it6161
#  set(CONFIG_USE_driver_display-it6161 true)

#  # description: Driver display-it6263
#  set(CONFIG_USE_driver_display-it6263 true)

#  # description: Driver fbdev
#  set(CONFIG_USE_driver_fbdev true)

#  # description: Driver video-common
#  set(CONFIG_USE_driver_video-common true)

#  # description: Driver video-i2c
#  set(CONFIG_USE_driver_video-i2c true)

#  # description: Component lpuart_adapter
#  set(CONFIG_USE_component_lpuart_adapter true)

#  # description: Component lpuart_dma_adapter
#  set(CONFIG_USE_component_lpuart_dma_adapter true)

#  # description: Driver ft5406_rt
#  set(CONFIG_USE_driver_ft5406_rt true)

#  # description: Driver ft6x06
#  set(CONFIG_USE_driver_ft6x06 true)

#  # description: Component timer_manager
#  set(CONFIG_USE_component_timer_manager true)

#  # description: Component gpt_adapter
#  set(CONFIG_USE_component_gpt_adapter true)

#  # description: Component pit_adapter
#  set(CONFIG_USE_component_pit_adapter true)

#  # description: Component lpspi_adapter
#  set(CONFIG_USE_component_lpspi_adapter true)

#  # description: Utility shell
#  set(CONFIG_USE_utility_shell true)

#  # description: Component serial_manager
#  set(CONFIG_USE_component_serial_manager true)

#  # description: Component serial_manager_spi
#  set(CONFIG_USE_component_serial_manager_spi true)

#  # description: Component serial_manager_uart
#  set(CONFIG_USE_component_serial_manager_uart true)

#  # description: Component serial_manager_usb_cdc
#  set(CONFIG_USE_component_serial_manager_usb_cdc true)

#  # description: Driver fxas21002cq
#  set(CONFIG_USE_driver_fxas21002cq true)

#  # description: Driver fxls8974cf
#  set(CONFIG_USE_driver_fxls8974cf true)

#  # description: Driver fxos8700cq
#  set(CONFIG_USE_driver_fxos8700cq true)

#  # description: Driver htu21d
#  set(CONFIG_USE_driver_htu21d true)

#  # description: Driver icm42688p
#  set(CONFIG_USE_driver_icm42688p true)

#  # description: Driver lsm6dso
#  set(CONFIG_USE_driver_lsm6dso true)

#  # description: Driver max30101
#  set(CONFIG_USE_driver_max30101 true)

#  # description: Driver mma8451q
#  set(CONFIG_USE_driver_mma8451q true)

#  # description: Driver mma8652fc
#  set(CONFIG_USE_driver_mma8652fc true)

#  # description: Driver p3t1755
#  set(CONFIG_USE_driver_p3t1755 true)

#  # description: Driver tsl2561
#  set(CONFIG_USE_driver_tsl2561 true)

#  # description: SEGGER Real Time Transfer(RTT)
#  set(CONFIG_USE_driver_rtt true)

#  # description: Component trng_adapter
#  set(CONFIG_USE_component_trng_adapter true)

#  # description: Driver pf1550
#  set(CONFIG_USE_driver_pf1550 true)

#  # description: Driver pf3000
#  set(CONFIG_USE_driver_pf3000 true)

#  # description: Driver pf5020
#  set(CONFIG_USE_driver_pf5020 true)

#  # description: Component mem_manager
#  set(CONFIG_USE_component_mem_manager true)

#  # description: Component mem_manager_legacy
#  set(CONFIG_USE_component_mem_manager_legacy true)

#  # description: Component mem_manager_freertos
#  set(CONFIG_USE_component_mem_manager_freertos true)

#  # description: Component mem_manager_light
#  set(CONFIG_USE_component_mem_manager_light true)

#  # description: Component log
#  set(CONFIG_USE_component_log true)

#  # description: Component log backend debug console
#  set(CONFIG_USE_component_log_backend_debug_console true)

#  # description: Component log backend debug console lite
#  set(CONFIG_USE_component_log_backend_debug_console_lite true)

#  # description: Component log backend ring buffer
#  set(CONFIG_USE_component_log_backend_ringbuffer true)

#  # description: Component lists
#  set(CONFIG_USE_component_lists true)

#  # description: Component led
#  set(CONFIG_USE_component_led true)

#  # description: Component lpi2c_adapter
#  set(CONFIG_USE_component_lpi2c_adapter true)

#  # description: Component i2c_adapter_interface
#  set(CONFIG_USE_component_i2c_adapter_interface true)

#  # description: Component igpio_adapter
#  set(CONFIG_USE_component_igpio_adapter true)

#  # description: Component.flash_nand_flexspi
#  set(CONFIG_USE_component_flash_nand_flexspi true)

#  # description: Component flash_nor_flexspi
#  set(CONFIG_USE_component_flash_nor_flexspi true)

#  # description: Component flash_nor_lpspi
#  set(CONFIG_USE_component_flash_nor_lpspi true)

#  # description: mflash onchip
#  set(CONFIG_USE_component_mflash_onchip true)

#  # description: mflash offchip
#  set(CONFIG_USE_component_mflash_offchip true)

#  # description: Component exception_handling_cm7
#  set(CONFIG_USE_component_exception_handling_cm7 true)

#  # description: Driver ili9341
#  set(CONFIG_USE_driver_ili9341 true)

#  # description: Driver psp27801
#  set(CONFIG_USE_driver_psp27801 true)

#  # description: Driver ssd1963
#  set(CONFIG_USE_driver_ssd1963 true)

#  # description: Driver st7796s
#  set(CONFIG_USE_driver_st7796s true)

#  # description: Component codec_i2c
#  set(CONFIG_USE_component_codec_i2c true)

#  # description: Driver ak4497
#  set(CONFIG_USE_driver_ak4497 true)

#  # description: Driver cs42448
#  set(CONFIG_USE_driver_cs42448 true)

#  # description: Driver cs42888
#  set(CONFIG_USE_driver_cs42888 true)

#  # description: Driver dialog7212
#  set(CONFIG_USE_driver_dialog7212 true)

#  # description: Driver pcm186x
#  set(CONFIG_USE_driver_pcm186x true)

#  # description: Driver pcm512x
#  set(CONFIG_USE_driver_pcm512x true)

#  # description: Driver sgtl5000
#  set(CONFIG_USE_driver_sgtl5000 true)

#  # description: Driver tfa9896
#  set(CONFIG_USE_driver_tfa9896 true)

#  # description: Driver tfa9xxx
#  set(CONFIG_USE_driver_tfa9xxx true)

#  # description: Driver tfa9xxx_hal
#  set(CONFIG_USE_driver_tfa9xxx_hal true)

#  # description: Driver wm8524
#  set(CONFIG_USE_driver_wm8524 true)

#  # description: Driver wm8904
#  set(CONFIG_USE_driver_wm8904 true)

#  # description: Driver wm8960
#  set(CONFIG_USE_driver_wm8960 true)

#  # description: Driver wm8962
#  set(CONFIG_USE_driver_wm8962 true)

#  # description: Component button
#  set(CONFIG_USE_component_button true)

#  # description: Component sai_edma_adapter
#  set(CONFIG_USE_component_audio_sai_edma_adapter true)

#set.CMSIS_DSP_Lib
#  # description: CMSIS-DSP Library Header
#  set(CONFIG_USE_CMSIS_DSP_Include true)

#  # description: CMSIS-DSP Library
#  set(CONFIG_USE_CMSIS_DSP_Source true)

#set.CMSIS
#  # description: Device interrupt controller interface
#  set(CONFIG_USE_CMSIS_Device_API_OSTick true)

#  # description: CMSIS-RTOS API for Cortex-M, SC000, and SC300
#  set(CONFIG_USE_CMSIS_Device_API_RTOS2 true)

#  # description: Access to #include Driver_CAN.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_CAN true)

#  # description: Access to #include Driver_ETH.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_Ethernet true)

#  # description: Access to #include Driver_ETH_MAC.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_Ethernet_MAC true)

#  # description: Access to #include Driver_ETH_PHY.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_Ethernet_PHY true)

#  # description: Access to #include Driver_Flash.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_Flash true)

#  # description: Access to #include Driver_GPIO.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_GPIO true)

#  # description: Access to #include Driver_I2C.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_I2C true)

#  # description: Access to #include Driver_MCI.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_MCI true)

#  # description: Access to #include Driver_NAND.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_NAND true)

#  # description: Access to #include Driver_SAI.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_SAI true)

#  # description: Access to #include Driver_SPI.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_SPI true)

#  # description: Access to #include Driver_USART.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_USART true)

#  # description: Access to #include Driver_USBD.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_USB_Device true)

#  # description: Access to #include Driver_USBH.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_USB_Host true)

#  # description: Access to #include Driver_WiFi.h file
#  set(CONFIG_USE_CMSIS_Driver_Include_WiFi true)

#  # description: CMSIS-NN Library
#  set(CONFIG_USE_CMSIS_NN_Source true)

#  # description: CMSIS-CORE for Cortex-M, ARMv8-M, ARMv8.1-M
#  set(CONFIG_USE_CMSIS_Include_core_cm true)

#  # description: CMSIS-RTOS2 RTX5 for Cortex-M, SC000, C300 and Armv8-M (Library)
#  set(CONFIG_USE_CMSIS_RTOS2_RTX true)

#  # description: CMSIS-RTOS2 RTX5 for Cortex-M, SC000, C300 and Armv8-M (Library)
#  set(CONFIG_USE_CMSIS_RTOS2_RTX_LIB true)

#set.middleware.tinycbor
#  # description: Concise Binary Object Representation (CBOR) Library
#  set(CONFIG_USE_middleware_tinycbor true)

#set.middleware.pkcs11
#  # description: Public Key Cryptography Standard #11 defines generic cryptographic token interface.
#  set(CONFIG_USE_middleware_pkcs11 true)

#set.middleware.lvgl
#  # description: lvgl template
#  set(CONFIG_USE_middleware_lvgl_template true)

#  # description: lvgl library
#  set(CONFIG_USE_middleware_lvgl true)

#  # description: LVGL benchmark demo
#  set(CONFIG_USE_middleware_lvgl_demo_benchmark true)

#  # description: LVGL demo common
#  set(CONFIG_USE_middleware_lvgl_demo_common true)

#  # description: LVGL demo transform
#  set(CONFIG_USE_middleware_lvgl_demo_transform true)

#  # description: LVGL demo multilang
#  set(CONFIG_USE_middleware_lvgl_demo_multilang true)

#  # description: LVGL demo flex_layout
#  set(CONFIG_USE_middleware_lvgl_demo_flex_layout true)

#  # description: LVGL demo scroll
#  set(CONFIG_USE_middleware_lvgl_demo_scroll true)

#  # description: LVGL demo music
#  set(CONFIG_USE_middleware_lvgl_demo_music true)

#  # description: LVGL demo keypad_encoder
#  set(CONFIG_USE_middleware_lvgl_demo_keypad_encoder true)

#  # description: LVGL stress demo
#  set(CONFIG_USE_middleware_lvgl_demo_stress true)

#  # description: LVGL widgets demo
#  set(CONFIG_USE_middleware_lvgl_demo_widgets true)

#set.middleware.libjpeg
#  # description: JPEG library
#  set(CONFIG_USE_middleware_libjpeg true)

#  # description: Template configuration file to be edited by user. Users could map the file system functions to the file system they are using.
#  set(CONFIG_USE_middleware_libjpeg_template true)

#set.middleware.fatfs
#  # description: FatFs
#  set(CONFIG_USE_middleware_fatfs true)

#  # description: FatFs template MMC
#  set(CONFIG_USE_middleware_fatfs_template_mmc true)

#  # description: FatFs template NAND
#  set(CONFIG_USE_middleware_fatfs_template_nand true)

#  # description: FatFs template RAM
#  set(CONFIG_USE_middleware_fatfs_template_ram true)

#  # description: FatFs template SD
#  set(CONFIG_USE_middleware_fatfs_template_sd true)

#  # description: FatFs template SDSPI
#  set(CONFIG_USE_middleware_fatfs_template_sdspi true)

#  # description: FatFs template USB
#  set(CONFIG_USE_middleware_fatfs_template_usb true)

#  # description: FatFs_RAM
#  set(CONFIG_USE_middleware_fatfs_ram true)

#  # description: FatFs_USB
#  set(CONFIG_USE_middleware_fatfs_usb true)

#set.middleware.emwin
#  # description: emwin template
#  set(CONFIG_USE_middleware_emwin_template true)

#  # description: emWin library
#  set(CONFIG_USE_middleware_emwin true)

#set.component.osa
#  # description: Component osa template config
#  set(CONFIG_USE_component_osa_template_config true)

#  # description: Component osa
#  set(CONFIG_USE_component_osa true)

#  # description: Component osa_bm
#  set(CONFIG_USE_component_osa_bm true)

#  # description: Component osa_free_rtos
#  set(CONFIG_USE_component_osa_free_rtos true)

#  # description: Component osa interface
#  set(CONFIG_USE_component_osa_interface true)

#  # description: Component common_task
#  set(CONFIG_USE_component_common_task true)

#set.middleware.freertos-kernel
#  # description: FreeRTOS kernel
#  set(CONFIG_USE_middleware_freertos-kernel true)

#  # description: FreeRTOS NXP extension
#  set(CONFIG_USE_middleware_freertos-kernel_extension true)

#  # description: FreeRTOS heap 1
#  set(CONFIG_USE_middleware_freertos-kernel_heap_1 true)

#  # description: FreeRTOS heap 2
#  set(CONFIG_USE_middleware_freertos-kernel_heap_2 true)

#  # description: FreeRTOS heap 3
#  set(CONFIG_USE_middleware_freertos-kernel_heap_3 true)

#  # description: FreeRTOS heap 4
#  set(CONFIG_USE_middleware_freertos-kernel_heap_4 true)

#  # description: FreeRTOS heap 5
#  set(CONFIG_USE_middleware_freertos-kernel_heap_5 true)

#  # description: old FreeRTOS MPU wrappers used before V10.6.0
#  set(CONFIG_USE_middleware_freertos-kernel_mpu_wrappers true)

#  # description: new V2 FreeRTOS MPU wrappers introduced in V10.6.0
#  set(CONFIG_USE_middleware_freertos-kernel_mpu_wrappers_v2 true)

#  # description: Template configuration file to be edited by user. Provides also memory allocator (heap_x), change variant if needed.
#  set(CONFIG_USE_middleware_freertos-kernel_config true)

#set.middleware.usb_pd
#  # description: Middleware usb_pd
#  set(CONFIG_USE_middleware_usb_pd true)

#  # description: Middleware usb_pd altmode
#  set(CONFIG_USE_middleware_usb_pd_altmode true)

#  # description: Middleware usb_pd phy ptn5110
#  set(CONFIG_USE_middleware_usb_pd_phy_ptn5110 true)

#  # description: Middleware usb_pd compliance_test_report
#  set(CONFIG_USE_middleware_usb_pd_compliance_test_report true)

#set.middleware.usb
#  # description: Middleware usb common_header
#  set(CONFIG_USE_middleware_usb_common_header true)

#  # description: Middleware usb device common_header
#  set(CONFIG_USE_middleware_usb_device_common_header true)

#  # description: Middleware usb device ehci
#  set(CONFIG_USE_middleware_usb_device_ehci true)

#  # description: Middleware usb device controller driver
#  set(CONFIG_USE_middleware_usb_device_controller_driver true)

#  # description: Middleware usb device stack
#  set(CONFIG_USE_middleware_usb_device_stack true)

#  # description: Middleware usb device audio
#  set(CONFIG_USE_middleware_usb_device_audio true)

#  # description: Middleware usb device cdc
#  set(CONFIG_USE_middleware_usb_device_cdc true)

#  # description: Middleware usb device cdc ecm
#  set(CONFIG_USE_middleware_usb_device_cdc_ecm true)

#  # description: Middleware usb device cdc rndis
#  set(CONFIG_USE_middleware_usb_device_cdc_rndis true)

#  # description: Middleware usb device hid
#  set(CONFIG_USE_middleware_usb_device_hid true)

#  # description: Middleware usb device dfu
#  set(CONFIG_USE_middleware_usb_device_dfu true)

#  # description: Middleware usb device msd
#  set(CONFIG_USE_middleware_usb_device_msd true)

#  # description: Middleware usb device mtp
#  set(CONFIG_USE_middleware_usb_device_mtp true)

#  # description: Middleware usb device phdc
#  set(CONFIG_USE_middleware_usb_device_phdc true)

#  # description: Middleware usb device video
#  set(CONFIG_USE_middleware_usb_device_video true)

#  # description: Middleware usb device ccid
#  set(CONFIG_USE_middleware_usb_device_ccid true)

#  # description: Middleware usb device printer
#  set(CONFIG_USE_middleware_usb_device_printer true)

#  # description: Middleware usb phy
#  set(CONFIG_USE_middleware_usb_phy true)

#  # description: Middleware usb PHYDCD (Select manually if needed)
#  set(CONFIG_USE_middleware_usb_phydcd true)

#  # description: Middleware usb host common_header
#  set(CONFIG_USE_middleware_usb_host_common_header true)

#  # description: Middleware usb host ehci
#  set(CONFIG_USE_middleware_usb_host_ehci true)

#  # description: Middleware usb host stack
#  set(CONFIG_USE_middleware_usb_host_stack true)

#  # description: Middleware usb host audio
#  set(CONFIG_USE_middleware_usb_host_audio true)

#  # description: Middleware usb host cdc
#  set(CONFIG_USE_middleware_usb_host_cdc true)

#  # description: Middleware usb host cdc_ecm
#  set(CONFIG_USE_middleware_usb_host_cdc_ecm true)

#  # description: Middleware usb host cdc_rndis
#  set(CONFIG_USE_middleware_usb_host_cdc_rndis true)

#  # description: Middleware usb host hid
#  set(CONFIG_USE_middleware_usb_host_hid true)

#  # description: Middleware usb host msd
#  set(CONFIG_USE_middleware_usb_host_msd true)

#  # description: Middleware usb host video
#  set(CONFIG_USE_middleware_usb_host_video true)

#  # description: Middleware usb host phdc
#  set(CONFIG_USE_middleware_usb_host_phdc true)

#  # description: Middleware usb host printer
#  set(CONFIG_USE_middleware_usb_host_printer true)

#set.middleware.llhttp
#  # description: HTTP parser in LLVM IR
#  set(CONFIG_USE_middleware_llhttp true)

#set.middleware.fmstr
#  # description: Common FreeMASTER driver code.
#  set(CONFIG_USE_middleware_fmstr true)

#  # description: FreeMASTER driver code for 32bit platforms, enabling communication between FreeMASTER or FreeMASTER Lite tools and MCU application. Supports Serial, CAN, USB and BDM/JTAG physical interface.
#  set(CONFIG_USE_middleware_fmstr_platform_gen32le true)

#  # description: FreeMASTER driver code for DSC platforms, enabling communication between FreeMASTER or FreeMASTER Lite tools and MCU application. Supports Serial, CAN, USB and BDM/JTAG physical interface.
#  set(CONFIG_USE_middleware_fmstr_platform_56f800e true)

#  # description: FreeMASTER driver code for S32 platform.
#  set(CONFIG_USE_middleware_fmstr_platform_s32 true)

#  # description: FreeMASTER driver code for Power Architecture 32bit platform.
#  set(CONFIG_USE_middleware_fmstr_platform_pa32 true)

#  # description: FreeMASTER driver code for S12Z platform.
#  set(CONFIG_USE_middleware_fmstr_platform_s12z true)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Core/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/DSP
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Driver
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/NN
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/evkmimxrt1015/project_template
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/evkmimxrt1015/xip
  ${CMAKE_CURRENT_LIST_DIR}/../../components/audio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/button
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/ak4497
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/cs42448
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/cs42888
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/da7212
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/i2c
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/pcm186x
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/pcm512x
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/sgtl5000
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/tfa9896
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/tfa9xxx
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/wm8524
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/wm8904
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/wm8960
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/wm8962
  ${CMAKE_CURRENT_LIST_DIR}/../../components/common_task
  ${CMAKE_CURRENT_LIST_DIR}/../../components/crc
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/ili9341
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/psp27801
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/ssd1963
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/st7796s
  ${CMAKE_CURRENT_LIST_DIR}/../../components/exception_handling
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/mflash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/gpio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/i2c
  ${CMAKE_CURRENT_LIST_DIR}/../../components/led
  ${CMAKE_CURRENT_LIST_DIR}/../../components/lists
  ${CMAKE_CURRENT_LIST_DIR}/../../components/log
  ${CMAKE_CURRENT_LIST_DIR}/../../components/mem_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/mx25r_flash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/osa
  ${CMAKE_CURRENT_LIST_DIR}/../../components/panic
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf1550
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf3000
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf5020
  ${CMAKE_CURRENT_LIST_DIR}/../../components/reset
  ${CMAKE_CURRENT_LIST_DIR}/../../components/rng
  ${CMAKE_CURRENT_LIST_DIR}/../../components/rtt
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/fxas21002cq
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/fxls8974cf
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/fxos8700cq
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/htu21d
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/icm42688p
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/lsm6dso
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/max30101
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/mma8451q
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/mma8652fc
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/p3t1755
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sensor/tsl2561
  ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/spi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/timer
  ${CMAKE_CURRENT_LIST_DIR}/../../components/timer_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ektf2k
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ft3267
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ft5406
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ft5406_rt
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ft6x06
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/gt911
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/tma525b
  ${CMAKE_CURRENT_LIST_DIR}/../../components/uart
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/ap1302
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/max9286
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/mt9m114
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/ov5640
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/ov7670
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/ov7725
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/sccb
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/receiver
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/receiver/flexio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/adv7535
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dbi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dbi/flexio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc/dbi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc/ssd1963
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/fbdev
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/it6161
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/it6263
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/sn65dsi83
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/i2c
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/emwin
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/fatfs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/freemaster
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/libjpeg
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/llhttp
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/benchmark
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/flex_layout
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/keypad_encoder
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/multilang
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/music
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/scroll
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/stress
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/transform
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/demos/widgets
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/pkcs11
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tinycbor
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/usb
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/freertos-kernel
  ${CMAKE_CURRENT_LIST_DIR}/cmsis_drivers
  ${CMAKE_CURRENT_LIST_DIR}/drivers
  ${CMAKE_CURRENT_LIST_DIR}/project_template
  ${CMAKE_CURRENT_LIST_DIR}/template
  ${CMAKE_CURRENT_LIST_DIR}/utilities
  ${CMAKE_CURRENT_LIST_DIR}/utilities/debug_console_lite
  ${CMAKE_CURRENT_LIST_DIR}/utilities/incbin
  ${CMAKE_CURRENT_LIST_DIR}/utilities/shell
  ${CMAKE_CURRENT_LIST_DIR}/xip
)

include_if_use(CMSIS_DSP_Include)
include_if_use(CMSIS_DSP_Source)
include_if_use(CMSIS_Device_API_OSTick)
include_if_use(CMSIS_Device_API_RTOS2)
include_if_use(CMSIS_Driver_Include_CAN)
include_if_use(CMSIS_Driver_Include_Ethernet)
include_if_use(CMSIS_Driver_Include_Ethernet_MAC)
include_if_use(CMSIS_Driver_Include_Ethernet_PHY)
include_if_use(CMSIS_Driver_Include_Flash)
include_if_use(CMSIS_Driver_Include_GPIO)
include_if_use(CMSIS_Driver_Include_I2C)
include_if_use(CMSIS_Driver_Include_MCI)
include_if_use(CMSIS_Driver_Include_NAND)
include_if_use(CMSIS_Driver_Include_SAI)
include_if_use(CMSIS_Driver_Include_SPI)
include_if_use(CMSIS_Driver_Include_USART)
include_if_use(CMSIS_Driver_Include_USB_Device)
include_if_use(CMSIS_Driver_Include_USB_Host)
include_if_use(CMSIS_Driver_Include_WiFi)
include_if_use(CMSIS_Include_core_cm)
include_if_use(CMSIS_NN_Source)
include_if_use(CMSIS_RTOS2_RTX)
include_if_use(CMSIS_RTOS2_RTX_LIB)
include_if_use(board_boot_header)
include_if_use(board_project_template)
include_if_use(component_ak4497_adapter.MIMXRT1015)
include_if_use(component_audio_sai_edma_adapter.MIMXRT1015)
include_if_use(component_button.MIMXRT1015)
include_if_use(component_codec_adapters.MIMXRT1015)
include_if_use(component_codec_i2c.MIMXRT1015)
include_if_use(component_common_task)
include_if_use(component_cs42448_adapter.MIMXRT1015)
include_if_use(component_cs42888_adapter.MIMXRT1015)
include_if_use(component_da7212_adapter.MIMXRT1015)
include_if_use(component_exception_handling_cm7.MIMXRT1015)
include_if_use(component_flash_nand_flexspi.MIMXRT1015)
include_if_use(component_flash_nor_flexspi.MIMXRT1015)
include_if_use(component_flash_nor_lpspi.MIMXRT1015)
include_if_use(component_gpt_adapter.MIMXRT1015)
include_if_use(component_i2c_adapter_interface.MIMXRT1015)
include_if_use(component_igpio_adapter.MIMXRT1015)
include_if_use(component_led.MIMXRT1015)
include_if_use(component_lists.MIMXRT1015)
include_if_use(component_log.MIMXRT1015)
include_if_use(component_log_backend_debug_console.MIMXRT1015)
include_if_use(component_log_backend_debug_console_lite.MIMXRT1015)
include_if_use(component_log_backend_ringbuffer.MIMXRT1015)
include_if_use(component_lpi2c_adapter.MIMXRT1015)
include_if_use(component_lpspi_adapter.MIMXRT1015)
include_if_use(component_lpuart_adapter.MIMXRT1015)
include_if_use(component_lpuart_dma_adapter.MIMXRT1015)
include_if_use(component_mem_manager.MIMXRT1015)
include_if_use(component_mem_manager_freertos.MIMXRT1015)
include_if_use(component_mem_manager_legacy.MIMXRT1015)
include_if_use(component_mem_manager_light.MIMXRT1015)
include_if_use(component_mflash_offchip.MIMXRT1015)
include_if_use(component_mflash_onchip.MIMXRT1015)
include_if_use(component_osa)
include_if_use(component_osa_bm)
include_if_use(component_osa_free_rtos)
include_if_use(component_osa_interface)
include_if_use(component_osa_template_config)
include_if_use(component_panic.MIMXRT1015)
include_if_use(component_pcm186x_adapter.MIMXRT1015)
include_if_use(component_pcm512x_adapter.MIMXRT1015)
include_if_use(component_pit_adapter.MIMXRT1015)
include_if_use(component_reset_adapter.MIMXRT1015)
include_if_use(component_serial_manager.MIMXRT1015)
include_if_use(component_serial_manager_spi.MIMXRT1015)
include_if_use(component_serial_manager_swo.MIMXRT1015)
include_if_use(component_serial_manager_uart.MIMXRT1015)
include_if_use(component_serial_manager_usb_cdc.MIMXRT1015)
include_if_use(component_serial_manager_virtual.MIMXRT1015)
include_if_use(component_sgtl_adapter.MIMXRT1015)
include_if_use(component_software_crc_adapter.MIMXRT1015)
include_if_use(component_software_rng_adapter.MIMXRT1015)
include_if_use(component_tfa9896_adapter.MIMXRT1015)
include_if_use(component_tfa9xxx_adapter.MIMXRT1015)
include_if_use(component_timer_manager.MIMXRT1015)
include_if_use(component_trng_adapter.MIMXRT1015)
include_if_use(component_wm8524_adapter.MIMXRT1015)
include_if_use(component_wm8904_adapter.MIMXRT1015)
include_if_use(component_wm8960_adapter.MIMXRT1015)
include_if_use(component_wm8962_adapter.MIMXRT1015)
include_if_use(device_CMSIS.MIMXRT1015)
include_if_use(device_RTE.MIMXRT1015)
include_if_use(device_boot_header.MIMXRT1015)
include_if_use(device_project_template.MIMXRT1015)
include_if_use(device_startup.MIMXRT1015)
include_if_use(device_system.MIMXRT1015)
include_if_use(driver_adc_12b1msps_sar.MIMXRT1015)
include_if_use(driver_adc_etc.MIMXRT1015)
include_if_use(driver_aipstz.MIMXRT1015)
include_if_use(driver_ak4497.MIMXRT1015)
include_if_use(driver_aoi.MIMXRT1015)
include_if_use(driver_bee.MIMXRT1015)
include_if_use(driver_cache_armv7_m7.MIMXRT1015)
include_if_use(driver_camera-common.MIMXRT1015)
include_if_use(driver_camera-device-ap1302.MIMXRT1015)
include_if_use(driver_camera-device-common.MIMXRT1015)
include_if_use(driver_camera-device-max9286.MIMXRT1015)
include_if_use(driver_camera-device-mt9m114.MIMXRT1015)
include_if_use(driver_camera-device-ov5640.MIMXRT1015)
include_if_use(driver_camera-device-ov7670.MIMXRT1015)
include_if_use(driver_camera-device-ov7725.MIMXRT1015)
include_if_use(driver_camera-device-sccb.MIMXRT1015)
include_if_use(driver_camera-receiver-common.MIMXRT1015)
include_if_use(driver_camera-receiver-flexio.MIMXRT1015)
include_if_use(driver_clock.MIMXRT1015)
include_if_use(driver_cmsis_lpi2c.MIMXRT1015)
include_if_use(driver_cmsis_lpspi.MIMXRT1015)
include_if_use(driver_cmsis_lpuart.MIMXRT1015)
include_if_use(driver_codec.MIMXRT1015)
include_if_use(driver_common.MIMXRT1015)
include_if_use(driver_cs42448.MIMXRT1015)
include_if_use(driver_cs42888.MIMXRT1015)
include_if_use(driver_dbi.MIMXRT1015)
include_if_use(driver_dbi_flexio_edma.MIMXRT1015)
include_if_use(driver_dc-fb-common.MIMXRT1015)
include_if_use(driver_dc-fb-dbi.MIMXRT1015)
include_if_use(driver_dc-fb-ssd1963.MIMXRT1015)
include_if_use(driver_dcdc_1.MIMXRT1015)
include_if_use(driver_dcp.MIMXRT1015)
include_if_use(driver_dialog7212.MIMXRT1015)
include_if_use(driver_display-adv7535.MIMXRT1015)
include_if_use(driver_display-common.MIMXRT1015)
include_if_use(driver_display-it6161.MIMXRT1015)
include_if_use(driver_display-it6263.MIMXRT1015)
include_if_use(driver_display-sn65dsi83.MIMXRT1015)
include_if_use(driver_dmamux.MIMXRT1015)
include_if_use(driver_edma.MIMXRT1015)
include_if_use(driver_ektf2k.MIMXRT1015)
include_if_use(driver_enc.MIMXRT1015)
include_if_use(driver_ewm.MIMXRT1015)
include_if_use(driver_fbdev.MIMXRT1015)
include_if_use(driver_flexio.MIMXRT1015)
include_if_use(driver_flexio_camera.MIMXRT1015)
include_if_use(driver_flexio_camera_edma.MIMXRT1015)
include_if_use(driver_flexio_i2c_master.MIMXRT1015)
include_if_use(driver_flexio_i2s.MIMXRT1015)
include_if_use(driver_flexio_i2s_edma.MIMXRT1015)
include_if_use(driver_flexio_mculcd.MIMXRT1015)
include_if_use(driver_flexio_mculcd_edma.MIMXRT1015)
include_if_use(driver_flexio_spi.MIMXRT1015)
include_if_use(driver_flexio_spi_edma.MIMXRT1015)
include_if_use(driver_flexio_uart.MIMXRT1015)
include_if_use(driver_flexio_uart_edma.MIMXRT1015)
include_if_use(driver_flexram.MIMXRT1015)
include_if_use(driver_flexram_allocate.MIMXRT1015)
include_if_use(driver_flexspi.MIMXRT1015)
include_if_use(driver_flexspi_edma.MIMXRT1015)
include_if_use(driver_ft3267.MIMXRT1015)
include_if_use(driver_ft5406.MIMXRT1015)
include_if_use(driver_ft5406_rt.MIMXRT1015)
include_if_use(driver_ft6x06.MIMXRT1015)
include_if_use(driver_fxas21002cq.MIMXRT1015)
include_if_use(driver_fxls8974cf.MIMXRT1015)
include_if_use(driver_fxos8700cq.MIMXRT1015)
include_if_use(driver_gpc_1.MIMXRT1015)
include_if_use(driver_gpt.MIMXRT1015)
include_if_use(driver_gt911.MIMXRT1015)
include_if_use(driver_htu21d.MIMXRT1015)
include_if_use(driver_icm42688p.MIMXRT1015)
include_if_use(driver_igpio.MIMXRT1015)
include_if_use(driver_ili9341.MIMXRT1015)
include_if_use(driver_iomuxc.MIMXRT1015)
include_if_use(driver_kpp.MIMXRT1015)
include_if_use(driver_lpi2c.MIMXRT1015)
include_if_use(driver_lpi2c_edma.MIMXRT1015)
include_if_use(driver_lpi2c_freertos.MIMXRT1015)
include_if_use(driver_lpspi.MIMXRT1015)
include_if_use(driver_lpspi_edma.MIMXRT1015)
include_if_use(driver_lpspi_freertos.MIMXRT1015)
include_if_use(driver_lpuart.MIMXRT1015)
include_if_use(driver_lpuart_edma.MIMXRT1015)
include_if_use(driver_lpuart_freertos.MIMXRT1015)
include_if_use(driver_lsm6dso.MIMXRT1015)
include_if_use(driver_max30101.MIMXRT1015)
include_if_use(driver_mma8451q.MIMXRT1015)
include_if_use(driver_mma8652fc.MIMXRT1015)
include_if_use(driver_mx25r_flash.MIMXRT1015)
include_if_use(driver_nic301.MIMXRT1015)
include_if_use(driver_ocotp.MIMXRT1015)
include_if_use(driver_p3t1755.MIMXRT1015)
include_if_use(driver_pcm186x.MIMXRT1015)
include_if_use(driver_pcm512x.MIMXRT1015)
include_if_use(driver_pf1550.MIMXRT1015)
include_if_use(driver_pf3000.MIMXRT1015)
include_if_use(driver_pf5020.MIMXRT1015)
include_if_use(driver_pit.MIMXRT1015)
include_if_use(driver_pmu.MIMXRT1015)
include_if_use(driver_psp27801.MIMXRT1015)
include_if_use(driver_pwm.MIMXRT1015)
include_if_use(driver_qtmr_1.MIMXRT1015)
include_if_use(driver_romapi.MIMXRT1015)
include_if_use(driver_rtt.MIMXRT1015)
include_if_use(driver_rtt_template.MIMXRT1015)
include_if_use(driver_rtwdog.MIMXRT1015)
include_if_use(driver_sai.MIMXRT1015)
include_if_use(driver_sai_edma.MIMXRT1015)
include_if_use(driver_sgtl5000.MIMXRT1015)
include_if_use(driver_snvs_hp.MIMXRT1015)
include_if_use(driver_snvs_lp.MIMXRT1015)
include_if_use(driver_spdif.MIMXRT1015)
include_if_use(driver_spdif_edma.MIMXRT1015)
include_if_use(driver_src.MIMXRT1015)
include_if_use(driver_ssd1963.MIMXRT1015)
include_if_use(driver_st7796s.MIMXRT1015)
include_if_use(driver_tempmon.MIMXRT1015)
include_if_use(driver_tfa9896.MIMXRT1015)
include_if_use(driver_tfa9xxx.MIMXRT1015)
include_if_use(driver_tfa9xxx_hal.MIMXRT1015)
include_if_use(driver_tma525b.MIMXRT1015)
include_if_use(driver_trng.MIMXRT1015)
include_if_use(driver_tsl2561.MIMXRT1015)
include_if_use(driver_video-common.MIMXRT1015)
include_if_use(driver_video-i2c.MIMXRT1015)
include_if_use(driver_wdog01.MIMXRT1015)
include_if_use(driver_wm8524.MIMXRT1015)
include_if_use(driver_wm8904.MIMXRT1015)
include_if_use(driver_wm8960.MIMXRT1015)
include_if_use(driver_wm8962.MIMXRT1015)
include_if_use(driver_xbara.MIMXRT1015)
include_if_use(driver_xbarb.MIMXRT1015)
include_if_use(middleware_emwin)
include_if_use(middleware_emwin_template)
include_if_use(middleware_fatfs)
include_if_use(middleware_fatfs_ram)
include_if_use(middleware_fatfs_template_mmc)
include_if_use(middleware_fatfs_template_nand)
include_if_use(middleware_fatfs_template_ram)
include_if_use(middleware_fatfs_template_sd)
include_if_use(middleware_fatfs_template_sdspi)
include_if_use(middleware_fatfs_template_usb)
include_if_use(middleware_fatfs_usb)
include_if_use(middleware_fmstr)
include_if_use(middleware_fmstr_platform_56f800e)
include_if_use(middleware_fmstr_platform_gen32le)
include_if_use(middleware_fmstr_platform_pa32)
include_if_use(middleware_fmstr_platform_s12z)
include_if_use(middleware_fmstr_platform_s32)
include_if_use(middleware_freertos-kernel)
include_if_use(middleware_freertos-kernel_config)
include_if_use(middleware_freertos-kernel_extension)
include_if_use(middleware_freertos-kernel_heap_1)
include_if_use(middleware_freertos-kernel_heap_2)
include_if_use(middleware_freertos-kernel_heap_3)
include_if_use(middleware_freertos-kernel_heap_4)
include_if_use(middleware_freertos-kernel_heap_5)
include_if_use(middleware_freertos-kernel_mpu_wrappers)
include_if_use(middleware_freertos-kernel_mpu_wrappers_v2)
include_if_use(middleware_libjpeg)
include_if_use(middleware_libjpeg_template)
include_if_use(middleware_llhttp)
include_if_use(middleware_lvgl)
include_if_use(middleware_lvgl_demo_benchmark)
include_if_use(middleware_lvgl_demo_common)
include_if_use(middleware_lvgl_demo_flex_layout)
include_if_use(middleware_lvgl_demo_keypad_encoder)
include_if_use(middleware_lvgl_demo_multilang)
include_if_use(middleware_lvgl_demo_music)
include_if_use(middleware_lvgl_demo_scroll)
include_if_use(middleware_lvgl_demo_stress)
include_if_use(middleware_lvgl_demo_transform)
include_if_use(middleware_lvgl_demo_widgets)
include_if_use(middleware_lvgl_template)
include_if_use(middleware_pkcs11)
include_if_use(middleware_tinycbor)
include_if_use(middleware_usb_common_header)
include_if_use(middleware_usb_device_audio)
include_if_use(middleware_usb_device_ccid)
include_if_use(middleware_usb_device_cdc)
include_if_use(middleware_usb_device_cdc_ecm)
include_if_use(middleware_usb_device_cdc_rndis)
include_if_use(middleware_usb_device_common_header)
include_if_use(middleware_usb_device_controller_driver)
include_if_use(middleware_usb_device_dfu)
include_if_use(middleware_usb_device_ehci)
include_if_use(middleware_usb_device_hid)
include_if_use(middleware_usb_device_msd)
include_if_use(middleware_usb_device_mtp)
include_if_use(middleware_usb_device_phdc)
include_if_use(middleware_usb_device_printer)
include_if_use(middleware_usb_device_stack)
include_if_use(middleware_usb_device_video)
include_if_use(middleware_usb_host_audio)
include_if_use(middleware_usb_host_cdc)
include_if_use(middleware_usb_host_cdc_ecm)
include_if_use(middleware_usb_host_cdc_rndis)
include_if_use(middleware_usb_host_common_header)
include_if_use(middleware_usb_host_ehci)
include_if_use(middleware_usb_host_hid)
include_if_use(middleware_usb_host_msd)
include_if_use(middleware_usb_host_phdc)
include_if_use(middleware_usb_host_printer)
include_if_use(middleware_usb_host_stack)
include_if_use(middleware_usb_host_video)
include_if_use(middleware_usb_pd)
include_if_use(middleware_usb_pd_altmode)
include_if_use(middleware_usb_pd_compliance_test_report)
include_if_use(middleware_usb_pd_phy_ptn5110)
include_if_use(middleware_usb_phy)
include_if_use(middleware_usb_phydcd)
include_if_use(utilities_misc_utilities.MIMXRT1015)
include_if_use(utility_assert.MIMXRT1015)
include_if_use(utility_assert_lite.MIMXRT1015)
include_if_use(utility_debug_console.MIMXRT1015)
include_if_use(utility_debug_console_lite.MIMXRT1015)
include_if_use(utility_debug_console_template_config.MIMXRT1015)
include_if_use(utility_incbin.MIMXRT1015)
include_if_use(utility_notifier.MIMXRT1015)
include_if_use(utility_shell.MIMXRT1015)
include_if_use(utility_str.MIMXRT1015)
