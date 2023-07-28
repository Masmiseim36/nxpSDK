# Copy variable into project config.cmake to use software component
#set.board.evkmimxrt1010
#  # description: Board_project_template evkmimxrt1010
#  set(CONFIG_USE_BOARD_Project_Template_evkmimxrt1010 true)

#  # description: XIP Board Driver
#  set(CONFIG_USE_driver_xip_board_evkmimxrt1010 true)

#set.device.MIMXRT1011
#  # description: Middleware baremetal
#  set(CONFIG_USE_middleware_baremetal true)

#  # description: Utilities which is needed for particular toolchain like the SBRK function required to address limitation between HEAP and STACK in GCC toolchain library.
#  set(CONFIG_USE_utilities_misc_utilities true)

#  # description: Driver nand_flash-common
#  set(CONFIG_USE_driver_nand_flash-common true)

#  # description: Driver nor_flash-common
#  set(CONFIG_USE_driver_nor_flash-common true)

#  # description: Driver mx25r_flash
#  set(CONFIG_USE_driver_mx25r_flash true)

#  # description: Driver pf1550
#  set(CONFIG_USE_driver_pf1550 true)

#  # description: Driver pf3000
#  set(CONFIG_USE_driver_pf3000 true)

#  # description: RTT template configuration
#  set(CONFIG_USE_driver_rtt_template true)

#  # description: Devices_project_template MIMXRT1011
#  set(CONFIG_USE_DEVICES_Project_Template_MIMXRT1011 true)

#  # description: Device MIMXRT1011_startup
#  set(CONFIG_USE_device_MIMXRT1011_startup true)

#  # description: Device MIMXRT1011_cmsis
#  set(CONFIG_USE_device_MIMXRT1011_CMSIS true)

#  # description: Rte_device
#  set(CONFIG_USE_RTE_Device true)

#  # description: SAI EDMA Driver
#  set(CONFIG_USE_driver_sai_edma true)

#  # description: LPI2C Driver
#  set(CONFIG_USE_driver_lpi2c_edma true)

#  # description: LPSPI Driver
#  set(CONFIG_USE_driver_lpspi_edma true)

#  # description: LPUART Driver
#  set(CONFIG_USE_driver_lpuart_edma true)

#  # description: DCP Driver
#  set(CONFIG_USE_driver_dcp true)

#  # description: Clock Driver
#  set(CONFIG_USE_driver_clock true)

#  # description: Component serial_manager_uart
#  set(CONFIG_USE_component_serial_manager_uart true)

#  # description: Utility debug_console_lite
#  set(CONFIG_USE_utility_debug_console_lite true)

#  # description: IOMUXC Driver
#  set(CONFIG_USE_driver_iomuxc true)

#  # description: LPSPI CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpspi true)

#  # description: LPI2C CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpi2c true)

#  # description: LPUART CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpuart true)

#  # description: FLEXIO CAMERA EDMA Driver
#  set(CONFIG_USE_driver_flexio_camera_edma true)

#  # description: FLEXIO I2S EDMA Driver
#  set(CONFIG_USE_driver_flexio_i2s_edma true)

#  # description: FLEXIO MCULCD EDMA Driver
#  set(CONFIG_USE_driver_flexio_mculcd_edma true)

#  # description: FLEXIO SPI EDMA Driver
#  set(CONFIG_USE_driver_flexio_spi_edma true)

#  # description: FLEXIO UART EDMA Driver
#  set(CONFIG_USE_driver_flexio_uart_edma true)

#  # description: FLEXSPI Driver
#  set(CONFIG_USE_driver_flexspi_edma true)

#  # description: SPDIF EDMA Driver
#  set(CONFIG_USE_driver_spdif_edma true)

#  # description: Utility notifier
#  set(CONFIG_USE_utility_notifier true)

#  # description: Device MIMXRT1011_system
#  set(CONFIG_USE_device_MIMXRT1011_system true)

#  # description: Utility assert
#  set(CONFIG_USE_utility_assert true)

#  # description: Utility assert_lite
#  set(CONFIG_USE_utility_assert_lite true)

#  # description: Utility str
#  set(CONFIG_USE_utility_str true)

#  # description: Utility debug_console
#  set(CONFIG_USE_utility_debug_console true)

#  # description: Component sai_edma_adapter
#  set(CONFIG_USE_component_audio_sai_edma_adapter true)

#  # description: Component button
#  set(CONFIG_USE_component_button true)

#  # description: Driver codec
#  set(CONFIG_USE_driver_codec true)

#  # description: Component codec adapters for multi codec
#  set(CONFIG_USE_component_codec_adapters true)

#  # description: Component wm8904 adapter for single codec
#  set(CONFIG_USE_component_wm8904_adapter true)

#  # description: Component wm8960 adapter for single codecs
#  set(CONFIG_USE_component_wm8960_adapter true)

#  # description: Component cs42888 adapter for single codec
#  set(CONFIG_USE_component_cs42888_adapter true)

#  # description: Component sgtl5000 adapter for single codec
#  set(CONFIG_USE_component_sgtl_adapter true)

#  # description: Component da7212 adapter for single codec
#  set(CONFIG_USE_component_da7212_adapter true)

#  # description: Component codec_i2c
#  set(CONFIG_USE_component_codec_i2c true)

#  # description: Component software_crc_adapter
#  set(CONFIG_USE_component_software_crc_adapter true)

#  # description: Driver cs42888
#  set(CONFIG_USE_driver_cs42888 true)

#  # description: Driver dialog7212
#  set(CONFIG_USE_driver_dialog7212 true)

#  # description: Component exception_handling_cm7
#  set(CONFIG_USE_component_exception_handling_cm7 true)

#  # description: Driver nand_flash-controller-flexspi
#  set(CONFIG_USE_driver_nand_flash-controller-flexspi true)

#  # description: Driver nor_flash-controller-flexspi
#  set(CONFIG_USE_driver_nor_flash-controller-flexspi true)

#  # description: Driver nor_flash-controller-lpspi
#  set(CONFIG_USE_driver_nor_flash-controller-lpspi true)

#  # description: Driver ft5406
#  set(CONFIG_USE_driver_ft5406 true)

#  # description: Driver ft5406_rt
#  set(CONFIG_USE_driver_ft5406_rt true)

#  # description: Driver ft6x06
#  set(CONFIG_USE_driver_ft6x06 true)

#  # description: Driver fxos8700cq
#  set(CONFIG_USE_driver_fxos8700cq true)

#  # description: Component igpio_adapter
#  set(CONFIG_USE_component_igpio_adapter true)

#  # description: Component lpi2c_adapter
#  set(CONFIG_USE_component_lpi2c_adapter true)

#  # description: Driver ili9341
#  set(CONFIG_USE_driver_ili9341 true)

#  # description: Component led
#  set(CONFIG_USE_component_led true)

#  # description: Component lists
#  set(CONFIG_USE_component_lists true)

#  # description: Component log
#  set(CONFIG_USE_component_log true)

#  # description: Component log backend debug console
#  set(CONFIG_USE_component_log_backend_debugconsole true)

#  # description: Component log backend debug console lite
#  set(CONFIG_USE_component_log_backend_debugconsole_lite true)

#  # description: Component log backend ring buffer
#  set(CONFIG_USE_component_log_backend_ringbuffer true)

#  # description: Component mem_manager
#  set(CONFIG_USE_component_mem_manager true)

#  # description: Component mem_manager_light
#  set(CONFIG_USE_component_mem_manager_light true)

#  # description: Driver mma8451q
#  set(CONFIG_USE_driver_mma8451q true)

#  # description: Component panic
#  set(CONFIG_USE_component_panic true)

#  # description: Component reset_adapter
#  set(CONFIG_USE_component_reset_adapter true)

#  # description: Component trng_adapter
#  set(CONFIG_USE_component_trng_adapter true)

#  # description: Component software_rng_adapter
#  set(CONFIG_USE_component_software_rng_adapter true)

#  # description: Component serial_manager
#  set(CONFIG_USE_component_serial_manager true)

#  # description: Component serial_manager_spi
#  set(CONFIG_USE_component_serial_manager_spi true)

#  # description: Component serial_manager_usb_cdc
#  set(CONFIG_USE_component_serial_manager_usb_cdc true)

#  # description: Component serial_manager_virtual
#  set(CONFIG_USE_component_serial_manager_virtual true)

#  # description: Component serial_manager_swo
#  set(CONFIG_USE_component_serial_manager_swo true)

#  # description: Driver sgtl5000
#  set(CONFIG_USE_driver_sgtl5000 true)

#  # description: Utility shell
#  set(CONFIG_USE_utility_shell true)

#  # description: Component lpspi_adapter
#  set(CONFIG_USE_component_lpspi_adapter true)

#  # description: Component gpt_adapter
#  set(CONFIG_USE_component_gpt_adapter true)

#  # description: Component pit_adapter
#  set(CONFIG_USE_component_pit_adapter true)

#  # description: Component timer_manager
#  set(CONFIG_USE_component_timer_manager true)

#  # description: Component lpuart_adapter
#  set(CONFIG_USE_component_lpuart_adapter true)

#  # description: Component lpuart_dma_adapter
#  set(CONFIG_USE_component_lpuart_dma_adapter true)

#  # description: Driver video-common
#  set(CONFIG_USE_driver_video-common true)

#  # description: Driver wm8904
#  set(CONFIG_USE_driver_wm8904 true)

#  # description: Driver wm8960
#  set(CONFIG_USE_driver_wm8960 true)

#  # description: COMMON Driver
#  set(CONFIG_USE_driver_common true)

#  # description: ADC Driver
#  set(CONFIG_USE_driver_adc_12b1msps_sar true)

#  # description: ADC_ETC Driver
#  set(CONFIG_USE_driver_adc_etc true)

#  # description: aipstz Driver
#  set(CONFIG_USE_driver_aipstz true)

#  # description: AOI Driver
#  set(CONFIG_USE_driver_aoi true)

#  # description: CACHE Driver
#  set(CONFIG_USE_driver_cache_armv7_m7 true)

#  # description: DCDC Driver
#  set(CONFIG_USE_driver_dcdc_1 true)

#  # description: DMAMUX Driver
#  set(CONFIG_USE_driver_dmamux true)

#  # description: EDMA Driver
#  set(CONFIG_USE_driver_edma true)

#  # description: EWM Driver
#  set(CONFIG_USE_driver_ewm true)

#  # description: FLEXIO Driver
#  set(CONFIG_USE_driver_flexio true)

#  # description: FLEXIO Driver
#  set(CONFIG_USE_driver_flexio_camera true)

#  # description: FLEXIO I2C Driver
#  set(CONFIG_USE_driver_flexio_i2c_master true)

#  # description: FLEXIO I2S Driver
#  set(CONFIG_USE_driver_flexio_i2s true)

#  # description: FLEXIO MCULCD Driver
#  set(CONFIG_USE_driver_flexio_mculcd true)

#  # description: FLEXIO SPI Driver
#  set(CONFIG_USE_driver_flexio_spi true)

#  # description: FLEXIO UART Driver
#  set(CONFIG_USE_driver_flexio_uart true)

#  # description: FLEXRAM Driver
#  set(CONFIG_USE_driver_flexram true)

#  # description: FLEXSPI Driver
#  set(CONFIG_USE_driver_flexspi true)

#  # description: GPC Driver
#  set(CONFIG_USE_driver_gpc_1 true)

#  # description: GPT Driver
#  set(CONFIG_USE_driver_gpt true)

#  # description: GPIO Driver
#  set(CONFIG_USE_driver_igpio true)

#  # description: KPP Driver
#  set(CONFIG_USE_driver_kpp true)

#  # description: LPI2C Driver
#  set(CONFIG_USE_driver_lpi2c true)

#  # description: LPI2C Driver
#  set(CONFIG_USE_driver_lpi2c_freertos true)

#  # description: LPSPI Driver
#  set(CONFIG_USE_driver_lpspi true)

#  # description: LPSPI Driver
#  set(CONFIG_USE_driver_lpspi_freertos true)

#  # description: LPUART Driver
#  set(CONFIG_USE_driver_lpuart true)

#  # description: LPUART Driver
#  set(CONFIG_USE_driver_lpuart_freertos true)

#  # description: OCOTP Driver
#  set(CONFIG_USE_driver_ocotp true)

#  # description: OTFAD Driver
#  set(CONFIG_USE_driver_otfad true)

#  # description: PIT Driver
#  set(CONFIG_USE_driver_pit true)

#  # description: pmu Driver
#  set(CONFIG_USE_driver_pmu true)

#  # description: PWM Driver
#  set(CONFIG_USE_driver_pwm true)

#  # description: RTWDOG Driver
#  set(CONFIG_USE_driver_rtwdog true)

#  # description: SAI Driver
#  set(CONFIG_USE_driver_sai true)

#  # description: SNVS HP Driver
#  set(CONFIG_USE_driver_snvs_hp true)

#  # description: SNVS LP Driver
#  set(CONFIG_USE_driver_snvs_lp true)

#  # description: SPDIF Driver
#  set(CONFIG_USE_driver_spdif true)

#  # description: SRC Driver
#  set(CONFIG_USE_driver_src true)

#  # description: TEMPMON Driver
#  set(CONFIG_USE_driver_tempmon true)

#  # description: TRNG Driver
#  set(CONFIG_USE_driver_trng true)

#  # description: wdog01 Driver
#  set(CONFIG_USE_driver_wdog01 true)

#  # description: XBARA Driver
#  set(CONFIG_USE_driver_xbara true)

#  # description: NIC301 Driver
#  set(CONFIG_USE_driver_nic301 true)

#  # description: SOC FLEXRAM ALLOCATE Driver
#  set(CONFIG_USE_driver_soc_flexram_allocate true)

#  # description: XIP Device Driver
#  set(CONFIG_USE_driver_xip_device true)

#  # description: SEGGER Real Time Transfer(RTT)
#  set(CONFIG_USE_driver_rtt true)

#set.CMSIS
#  # description: CMSIS-CORE for Cortex-M, ARMv8-M, ARMv8.1-M
#  set(CONFIG_USE_CMSIS_Include_core_cm true)

#  # description: Access to #include Driver_USART.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_USART true)

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

#  # description: Access to #include Driver_USBD.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_USB_Device true)

#  # description: Access to #include Driver_USBH.h file for custom implementation
#  set(CONFIG_USE_CMSIS_Driver_Include_USB_Host true)

#  # description: Access to #include Driver_WiFi.h file
#  set(CONFIG_USE_CMSIS_Driver_Include_WiFi true)

#  # description: Device interrupt controller interface
#  set(CONFIG_USE_CMSIS_Device_API_OSTick true)

#  # description: CMSIS-RTOS API for Cortex-M, SC000, and SC300
#  set(CONFIG_USE_CMSIS_Device_API_RTOS2 true)

#  # description: CMSIS-RTOS2 RTX5 for Cortex-M, SC000, C300 and Armv8-M (Library)
#  set(CONFIG_USE_CMSIS_RTOS2_Secure true)

#  # description: CMSIS-RTOS2 RTX5 for Armv8-M Non-Secure Domain (Library)
#  set(CONFIG_USE_CMSIS_RTOS2_NonSecure true)

#set.CMSIS_DSP_Lib
#  # description: CMSIS-DSP Library Header
#  set(CONFIG_USE_CMSIS_DSP_Include true)

#  # description: CMSIS-DSP Library
#  set(CONFIG_USE_CMSIS_DSP_Source true)

#  # description: CMSIS-NN Library
#  set(CONFIG_USE_CMSIS_NN_Source true)

#set.middleware.embedded_wizard
#  # description: embedded_wizard template
#  set(CONFIG_USE_middleware_embedded_wizard_template_evkmimxrt1010 true)

#  # description: embedded_wizard library
#  set(CONFIG_USE_middleware_embedded_wizard_evkmimxrt1010 true)

#  # description: embedded_wizard library
#  set(CONFIG_USE_middleware_embedded_wizard_lib_evkmimxrt10xx true)

#set.middleware.emwin
#  # description: emwin template
#  set(CONFIG_USE_middleware_emwin_template true)

#  # description: emWin library
#  set(CONFIG_USE_middleware_emwin true)

#set.middleware.fatfs
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

#  # description: FatFs
#  set(CONFIG_USE_middleware_fatfs true)

#  # description: FatFs_RAM
#  set(CONFIG_USE_middleware_fatfs_ram true)

#  # description: FatFs_USB
#  set(CONFIG_USE_middleware_fatfs_usb true)

#set.middleware.freertos-kernel
#  # description: FreeRTOS NXP extension
#  set(CONFIG_USE_middleware_freertos-kernel_extension true)

#  # description: Template configuration file to be edited by user. Provides also memory allocator (heap_x), change variant if needed.
#  set(CONFIG_USE_middleware_freertos-kernel_template true)

#  # description: FreeRTOS kernel
#  set(CONFIG_USE_middleware_freertos-kernel true)

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

#  # description: FreeRTOS MPU wrappers
#  set(CONFIG_USE_middleware_freertos-kernel_mpu_wrappers true)

#set.middleware.libjpeg
#  # description: Template configuration file to be edited by user. Users could map the file system functions to the file system they are using.
#  set(CONFIG_USE_middleware_libjpeg_template true)

#  # description: JPEG library
#  set(CONFIG_USE_middleware_libjpeg true)

#set.middleware.pkcs11
#  # description: Public Key Cryptography Standard #11 defines generic cryptographic token interface.
#  set(CONFIG_USE_middleware_pkcs11 true)

#set.middleware.tinycbor
#  # description: Concise Binary Object Representation (CBOR) Library
#  set(CONFIG_USE_middleware_tinycbor true)

#set.middleware.mbedtls
#  # description: mbedTLS Template
#  set(CONFIG_USE_middleware_mbedtls_template true)

#  # description: els_pkc config
#  set(CONFIG_USE_middleware_mbedtls_els_pkc_config true)

#  # description: mbedTLS port library for KPSDK
#  set(CONFIG_USE_middleware_mbedtls_port_ksdk true)

#  # description: mbedTLS library
#  set(CONFIG_USE_middleware_mbedtls true)

#set.middleware.lvgl
#  # description: lvgl template
#  set(CONFIG_USE_middleware_lvgl_template true)

#  # description: LVGL files unused by projects
#  set(CONFIG_USE_middleware_lvgl_unused_files true)

#  # description: lvgl library
#  set(CONFIG_USE_middleware_lvgl true)

#  # description: LVGL benchmark demo
#  set(CONFIG_USE_middleware_lvgl_demo_benchmark true)

#  # description: LVGL widgets demo
#  set(CONFIG_USE_middleware_lvgl_demo_widgets true)

#  # description: LVGL stress demo
#  set(CONFIG_USE_middleware_lvgl_demo_stress true)

#set.middleware.mcu-boot
#  # description: CMSIS cm7 headers
#  set(CONFIG_USE_middleware_mcu-boot_CMSIS_cm7_headers true)

#  # description: MCUBOOT microseconds sysclk
#  set(CONFIG_USE_middleware_mcu-boot_drv_microseconds true)

#  # description: MCUBOOT microseconds pit
#  set(CONFIG_USE_middleware_mcu-boot_drv_microseconds_pit true)

#  # description: MCUBOOT microseconds lpit
#  set(CONFIG_USE_middleware_mcu-boot_drv_microseconds_lpit true)

#  # description: Bootloader common sources
#  set(CONFIG_USE_middleware_mcu-boot_bootsources true)

#  # description: Bootloader startup sources
#  set(CONFIG_USE_middleware_mcu-boot_src_startup true)

#  # description: imxrt files
#  set(CONFIG_USE_middleware_mcu-boot_imxrt_files true)

#  # description: sdphost files
#  set(CONFIG_USE_middleware_mcu-boot_sdphost true)

#  # description: imxrt1010 files
#  set(CONFIG_USE_middleware_mcu-boot_imxrt1010_files true)

#  # description: Bootloader bm usb sources
#  set(CONFIG_USE_middleware_mcu-boot_src_bm_usb_hid true)

#  # description: MCUBOOT ocotp driver
#  set(CONFIG_USE_middleware_mcu-boot_drv_ocotp true)

#  # description: MCUBOOT flexspi nand driver
#  set(CONFIG_USE_middleware_mcu-boot_drv_flexspi_nand true)

#  # description: MCUBOOT spi nand memory interface
#  set(CONFIG_USE_middleware_mcu-boot_mem_spi_nand true)

#  # description: MCUBOOT edgelock driver
#  set(CONFIG_USE_middleware_mcu-boot_drv_edgelock true)

#  # description: MIMXRT1011 drivers
#  set(CONFIG_USE_middleware_mcu-boot_MIMXRT1011_drivers true)

#  # description: MCUBOOT MIMXRT1011 startup sources
#  set(CONFIG_USE_middleware_mcu-boot_MIMXRT1011_startup true)

#  # description: MCUBOOT MIMXRT1011 sources
#  set(CONFIG_USE_middleware_mcu-boot_MIMXRT1011_sources true)

#set.middleware.issdk
#  # description: ISSDK Serial to MQTT bridge application
#  set(CONFIG_USE_middleware_issdk_sensor_tools true)

#  # description: ISSDK Utils
#  set(CONFIG_USE_middleware_issdk_drivers_systick_utils true)

#  # description: ISSDK Sensors RegDefs
#  set(CONFIG_USE_middleware_issdk_sensor_allregdefs true)

#  # description: ISSDK FXAS21002 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxas21002 true)

#  # description: ISSDK FXOS8700 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxos8700 true)

#  # description: ISSDK MPL3115 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_mpl3115 true)

#  # description: ISSDK FXLC95000 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxlc95000 true)

#  # description: ISSDK MMA8491 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_mma8491q true)

#  # description: ISSDK FXLS8471 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxls8471q true)

#  # description: ISSDK MAG3110 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_mag3110 true)

#  # description: ISSDK MMA845x Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_mma845x true)

#  # description: ISSDK MMA865x Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_mma865x true)

#  # description: ISSDK FXPQ3115 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxpq3115 true)

#  # description: ISSDK FXLS8962 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxls8962 true)

#  # description: ISSDK MMA9553 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_mma9553 true)

#  # description: ISSDK nps300x Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_nps300x true)

#  # description: ISSDK ISL29023 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_isl29023 true)

#  # description: ISSDK fxps7250d4 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxps7250d4 true)

#  # description: ISSDK FXLS896xAF Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxls896xaf true)

#  # description: ISSDK FXLS8974CF Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxls8974cf true)

#  # description: ISSDK FXLS8961AF Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxls8961af true)

#  # description: ISSDK FXLS8971CF Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxls8971cf true)

#  # description: ISSDK NMH1000 Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_nmh1000 true)

#  # description: ISSDK Sensor Interface Common
#  set(CONFIG_USE_middleware_issdk_sensor_interface_common true)

#  # description: ISSDK GPIO Driver for i.MXRT10xx
#  set(CONFIG_USE_middleware_issdk_drivers_gpio_imx true)

#  # description: ISSDK GPIO Driver for i.MX8xx
#  set(CONFIG_USE_middleware_issdk_drivers_gpio_imx8 true)

#  # description: ISSDK GPIO Driver for i.MXRT685
#  set(CONFIG_USE_middleware_issdk_drivers_gpio_imxrt600 true)

#  # description: ISSDK GPIO Driver for i.MXRT1180
#  set(CONFIG_USE_middleware_issdk_drivers_gpio_imxrt1180 true)

#  # description: ISSDK GPIO Driver for i.MXRT
#  set(CONFIG_USE_middleware_issdk_drivers_gpio_imxrt true)

#  # description: ISSDK Host Interface Service
#  set(CONFIG_USE_middleware_issdk_drivers_host true)

#set.middleware.fmstr
#  # description: Common FreeMASTER driver code.
#  set(CONFIG_USE_middleware_fmstr true)

#  # description: DO NOT ADD THIS TO PROJECT. This component contains all FreeMASTER driver source code files for SDK distribution purpose. Not suitable for project build.
#  set(CONFIG_USE_middleware_fmstr_all_files true)

#  # description: FreeMASTER driver code for 32bit platforms, enabling communication between FreeMASTER or FreeMASTER Lite tools and MCU application. Supports Serial, CAN, USB and BDM/JTAG physical interface.
#  set(CONFIG_USE_middleware_fmstr_platform_gen32le true)

#  # description: FreeMASTER configuration file template. Contains placeholders and TODOs which need to be filled by user or by using Peripherals ConfigTool.
#  set(CONFIG_USE_middleware_fmstr_npw_cfg_gen32le true)

#set.middleware.azure_rtos
#  # description: Azure RTOS Core
#  set(CONFIG_USE_middleware_azure_rtos_tx_template true)

#  # description: Azure RTOS Core
#  set(CONFIG_USE_middleware_azure_rtos_tx_mgr_template true)

#  # description: Real Time Operating System Kernel
#  set(CONFIG_USE_middleware_azure_rtos_tx_lib true)

#  # description: Real Time Operating System Kernel
#  set(CONFIG_USE_middleware_azure_rtos_tx_mgr_lib true)

#  # description: Real Time Operating System Kernel
#  set(CONFIG_USE_middleware_azure_rtos_txm_lib true)

#  # description: A file system based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_fx_template true)

#  # description: A file system based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_fx_lib true)

#  # description: A GUI library based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_gx_lib true)

#  # description: A network protocol stack based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_nxd_template true)

#  # description: A network protocol stack based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_nxd_lib true)

#  # description: A USB library based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_ux_template true)

#  # description: Azure RTOS USBX Host Controller EHCI
#  set(CONFIG_USE_middleware_azure_rtos_ux_ehci true)

#  # description: Azure RTOS USBX Host Controller OHCI
#  set(CONFIG_USE_middleware_azure_rtos_ux_ohci true)

#  # description: Azure RTOS USBX Host Controller IP3516
#  set(CONFIG_USE_middleware_azure_rtos_ux_ip3516 true)

#  # description: Azure RTOS USBX Device Controller IP3511
#  set(CONFIG_USE_middleware_azure_rtos_ux_ip3511 true)

#  # description: Azure RTOS USBX Device Controller for i.MX RT
#  set(CONFIG_USE_middleware_azure_rtos_ux_dci true)

#  # description: A USB library based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_ux_lib true)

#  # description: Azure RTOS Core
#  set(CONFIG_USE_middleware_azure_rtos_tx true)

#  # description: Azure RTOS Core
#  set(CONFIG_USE_middleware_azure_rtos_tx_mgr true)

#  # description: Azure RTOS Core
#  set(CONFIG_USE_middleware_azure_rtos_tx_sp true)

#  # description: A file system based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_fx true)

#  # description: A file system based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_fx_sp true)

#  # description: A GUI library based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_gx true)

#  # description: LevelX provides NAND and NOR flash wear leveling facilities to embedded applications
#  set(CONFIG_USE_middleware_azure_rtos_lx true)

#  # description: A network protocol stack based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_nxd true)

#  # description: A network protocol stack based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_nxd_sp true)

#  # description: Azure NetX Duo driver based on i.MXRT series
#  set(CONFIG_USE_middleware_netxduo_imxrt true)

#  # description: A USB library based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_ux true)

#  # description: A USB library based on azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_ux_sp true)

#  # description: A software package that connects to the IoT Hub through Azure RTOS
#  set(CONFIG_USE_middleware_azure_rtos_azure_iot true)

#set.middleware.llhttp
#  # description: HTTP parser in LLVM IR
#  set(CONFIG_USE_middleware_llhttp true)

#set.middleware.usb
#  # description: USB device ehci config header
#  set(CONFIG_USE_middleware_usb_device_ehci_config_header true)

#  # description: Middleware usb common_header
#  set(CONFIG_USE_middleware_usb_common_header true)

#  # description: Middleware usb device common_header
#  set(CONFIG_USE_middleware_usb_device_common_header true)

#  # description: Middleware usb device ehci
#  set(CONFIG_USE_middleware_usb_device_ehci true)

#  # description: Middleware usb phy
#  set(CONFIG_USE_middleware_usb_phy true)

#  # description: Middleware usb device stack external
#  set(CONFIG_USE_middleware_usb_device_stack_external true)

#  # description: Middleware usb device audio external
#  set(CONFIG_USE_middleware_usb_device_audio_external true)

#  # description: Middleware usb device cdc external
#  set(CONFIG_USE_middleware_usb_device_cdc_external true)

#  # description: Middleware usb device cdc rndis external
#  set(CONFIG_USE_middleware_usb_device_cdc_rndis_external true)

#  # description: Middleware usb device hid external
#  set(CONFIG_USE_middleware_usb_device_hid_external true)

#  # description: Middleware usb device dfu external
#  set(CONFIG_USE_middleware_usb_device_dfu_external true)

#  # description: Middleware usb device msd external
#  set(CONFIG_USE_middleware_usb_device_msd_external true)

#  # description: Middleware usb device phdc external
#  set(CONFIG_USE_middleware_usb_device_phdc_external true)

#  # description: Middleware usb device video external
#  set(CONFIG_USE_middleware_usb_device_video_external true)

#  # description: Middleware usb device ccid external
#  set(CONFIG_USE_middleware_usb_device_ccid_external true)

#  # description: Middleware usb device printer external
#  set(CONFIG_USE_middleware_usb_device_printer_external true)

#  # description: Middleware usb device controller driver
#  set(CONFIG_USE_middleware_usb_device_controller_driver true)

#  # description: Middleware usb host ehci
#  set(CONFIG_USE_middleware_usb_host_ehci true)

#  # description: Middleware usb host audio
#  set(CONFIG_USE_middleware_usb_host_audio true)

#  # description: Middleware usb host cdc
#  set(CONFIG_USE_middleware_usb_host_cdc true)

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

#  # description: Middleware usb host common_header
#  set(CONFIG_USE_middleware_usb_host_common_header true)

#  # description: USB host ehci config header
#  set(CONFIG_USE_middleware_usb_host_ehci_config_header true)

#  # description: Middleware usb host stack
#  set(CONFIG_USE_middleware_usb_host_stack true)

#set.component.osa
#  # description: Component common_task
#  set(CONFIG_USE_component_common_task true)

#  # description: Component osa_bm
#  set(CONFIG_USE_component_osa_bm true)

#  # description: Component osa_free_rtos
#  set(CONFIG_USE_component_osa_free_rtos true)

#  # description: Component osa
#  set(CONFIG_USE_component_osa true)

#  # description: Component osa interface
#  set(CONFIG_USE_component_osa_interface true)

#  # description: Component osa thread
#  set(CONFIG_USE_component_osa_thread true)

#set.component.serial_mwm
#  # description: Host driver middleware and API's for connecting to MW320 Wi-Fi co-processor with on-board TCP/IP stack
#  set(CONFIG_USE_component_serial_mwm true)

#  # description: LPUART port layer for Serial MWM
#  set(CONFIG_USE_component_serial_mwm_lpuart true)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Core/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/DSP
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Driver
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/NN
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/evkmimxrt1010/project_template
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/evkmimxrt1010/xip
  ${CMAKE_CURRENT_LIST_DIR}/../../components/audio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/button
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/cs42888
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/da7212
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/i2c
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/sgtl5000
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/wm8904
  ${CMAKE_CURRENT_LIST_DIR}/../../components/codec/wm8960
  ${CMAKE_CURRENT_LIST_DIR}/../../components/common_task
  ${CMAKE_CURRENT_LIST_DIR}/../../components/crc
  ${CMAKE_CURRENT_LIST_DIR}/../../components/exception_handling
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/nand
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/nand/flexspi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/nor
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/nor/flexspi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/nor/lpspi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/ft5406
  ${CMAKE_CURRENT_LIST_DIR}/../../components/ft5406_rt
  ${CMAKE_CURRENT_LIST_DIR}/../../components/ft6x06
  ${CMAKE_CURRENT_LIST_DIR}/../../components/fxos8700cq
  ${CMAKE_CURRENT_LIST_DIR}/../../components/gpio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/i2c
  ${CMAKE_CURRENT_LIST_DIR}/../../components/ili9341
  ${CMAKE_CURRENT_LIST_DIR}/../../components/led
  ${CMAKE_CURRENT_LIST_DIR}/../../components/lists
  ${CMAKE_CURRENT_LIST_DIR}/../../components/log
  ${CMAKE_CURRENT_LIST_DIR}/../../components/mem_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/mma8451q
  ${CMAKE_CURRENT_LIST_DIR}/../../components/mx25r_flash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/osa
  ${CMAKE_CURRENT_LIST_DIR}/../../components/panic
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pf1550
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pf3000
  ${CMAKE_CURRENT_LIST_DIR}/../../components/reset
  ${CMAKE_CURRENT_LIST_DIR}/../../components/rng
  ${CMAKE_CURRENT_LIST_DIR}/../../components/rtt
  ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_mwm
  ${CMAKE_CURRENT_LIST_DIR}/../../components/spi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/timer
  ${CMAKE_CURRENT_LIST_DIR}/../../components/timer_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/uart
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/EmbeddedWizard
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/bm
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/emwin
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/fatfs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/freemaster
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/issdk
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/libjpeg
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/llhttp
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lvgl/lvgl_sdk
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mbedtls
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcu_bootloader
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/pkcs11
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tinycbor
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/usb
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/azure-rtos
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/freertos-kernel
  ${CMAKE_CURRENT_LIST_DIR}/cmsis_drivers
  ${CMAKE_CURRENT_LIST_DIR}/drivers
  ${CMAKE_CURRENT_LIST_DIR}/project_template
  ${CMAKE_CURRENT_LIST_DIR}/template
  ${CMAKE_CURRENT_LIST_DIR}/utilities
  ${CMAKE_CURRENT_LIST_DIR}/utilities/debug_console_lite
  ${CMAKE_CURRENT_LIST_DIR}/xip
)

include_if_use(BOARD_Project_Template_evkmimxrt1010)
include_if_use(CMSIS_DSP_Include)
include_if_use(CMSIS_DSP_Source)
include_if_use(CMSIS_Device_API_OSTick)
include_if_use(CMSIS_Device_API_RTOS2)
include_if_use(CMSIS_Driver_Include_CAN)
include_if_use(CMSIS_Driver_Include_Ethernet)
include_if_use(CMSIS_Driver_Include_Ethernet_MAC)
include_if_use(CMSIS_Driver_Include_Ethernet_PHY)
include_if_use(CMSIS_Driver_Include_Flash)
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
include_if_use(CMSIS_RTOS2_NonSecure)
include_if_use(CMSIS_RTOS2_Secure)
include_if_use(DEVICES_Project_Template_MIMXRT1011.MIMXRT1011)
include_if_use(RTE_Device.MIMXRT1011)
include_if_use(component_audio_sai_edma_adapter.MIMXRT1011)
include_if_use(component_button.MIMXRT1011)
include_if_use(component_codec_adapters.MIMXRT1011)
include_if_use(component_codec_i2c.MIMXRT1011)
include_if_use(component_common_task)
include_if_use(component_cs42888_adapter.MIMXRT1011)
include_if_use(component_da7212_adapter.MIMXRT1011)
include_if_use(component_exception_handling_cm7.MIMXRT1011)
include_if_use(component_gpt_adapter.MIMXRT1011)
include_if_use(component_igpio_adapter.MIMXRT1011)
include_if_use(component_led.MIMXRT1011)
include_if_use(component_lists.MIMXRT1011)
include_if_use(component_log.MIMXRT1011)
include_if_use(component_log_backend_debugconsole.MIMXRT1011)
include_if_use(component_log_backend_debugconsole_lite.MIMXRT1011)
include_if_use(component_log_backend_ringbuffer.MIMXRT1011)
include_if_use(component_lpi2c_adapter.MIMXRT1011)
include_if_use(component_lpspi_adapter.MIMXRT1011)
include_if_use(component_lpuart_adapter.MIMXRT1011)
include_if_use(component_lpuart_dma_adapter.MIMXRT1011)
include_if_use(component_mem_manager.MIMXRT1011)
include_if_use(component_mem_manager_light.MIMXRT1011)
include_if_use(component_osa)
include_if_use(component_osa_bm)
include_if_use(component_osa_free_rtos)
include_if_use(component_osa_interface)
include_if_use(component_osa_thread)
include_if_use(component_panic.MIMXRT1011)
include_if_use(component_pit_adapter.MIMXRT1011)
include_if_use(component_reset_adapter.MIMXRT1011)
include_if_use(component_serial_manager.MIMXRT1011)
include_if_use(component_serial_manager_spi.MIMXRT1011)
include_if_use(component_serial_manager_swo.MIMXRT1011)
include_if_use(component_serial_manager_uart.MIMXRT1011)
include_if_use(component_serial_manager_usb_cdc.MIMXRT1011)
include_if_use(component_serial_manager_virtual.MIMXRT1011)
include_if_use(component_serial_mwm)
include_if_use(component_serial_mwm_lpuart)
include_if_use(component_sgtl_adapter.MIMXRT1011)
include_if_use(component_software_crc_adapter.MIMXRT1011)
include_if_use(component_software_rng_adapter.MIMXRT1011)
include_if_use(component_timer_manager.MIMXRT1011)
include_if_use(component_trng_adapter.MIMXRT1011)
include_if_use(component_wm8904_adapter.MIMXRT1011)
include_if_use(component_wm8960_adapter.MIMXRT1011)
include_if_use(device_MIMXRT1011_CMSIS.MIMXRT1011)
include_if_use(device_MIMXRT1011_startup.MIMXRT1011)
include_if_use(device_MIMXRT1011_system.MIMXRT1011)
include_if_use(driver_adc_12b1msps_sar.MIMXRT1011)
include_if_use(driver_adc_etc.MIMXRT1011)
include_if_use(driver_aipstz.MIMXRT1011)
include_if_use(driver_aoi.MIMXRT1011)
include_if_use(driver_cache_armv7_m7.MIMXRT1011)
include_if_use(driver_clock.MIMXRT1011)
include_if_use(driver_cmsis_lpi2c.MIMXRT1011)
include_if_use(driver_cmsis_lpspi.MIMXRT1011)
include_if_use(driver_cmsis_lpuart.MIMXRT1011)
include_if_use(driver_codec.MIMXRT1011)
include_if_use(driver_common.MIMXRT1011)
include_if_use(driver_cs42888.MIMXRT1011)
include_if_use(driver_dcdc_1.MIMXRT1011)
include_if_use(driver_dcp.MIMXRT1011)
include_if_use(driver_dialog7212.MIMXRT1011)
include_if_use(driver_dmamux.MIMXRT1011)
include_if_use(driver_edma.MIMXRT1011)
include_if_use(driver_ewm.MIMXRT1011)
include_if_use(driver_flexio.MIMXRT1011)
include_if_use(driver_flexio_camera.MIMXRT1011)
include_if_use(driver_flexio_camera_edma.MIMXRT1011)
include_if_use(driver_flexio_i2c_master.MIMXRT1011)
include_if_use(driver_flexio_i2s.MIMXRT1011)
include_if_use(driver_flexio_i2s_edma.MIMXRT1011)
include_if_use(driver_flexio_mculcd.MIMXRT1011)
include_if_use(driver_flexio_mculcd_edma.MIMXRT1011)
include_if_use(driver_flexio_spi.MIMXRT1011)
include_if_use(driver_flexio_spi_edma.MIMXRT1011)
include_if_use(driver_flexio_uart.MIMXRT1011)
include_if_use(driver_flexio_uart_edma.MIMXRT1011)
include_if_use(driver_flexram.MIMXRT1011)
include_if_use(driver_flexspi.MIMXRT1011)
include_if_use(driver_flexspi_edma.MIMXRT1011)
include_if_use(driver_ft5406.MIMXRT1011)
include_if_use(driver_ft5406_rt.MIMXRT1011)
include_if_use(driver_ft6x06.MIMXRT1011)
include_if_use(driver_fxos8700cq.MIMXRT1011)
include_if_use(driver_gpc_1.MIMXRT1011)
include_if_use(driver_gpt.MIMXRT1011)
include_if_use(driver_igpio.MIMXRT1011)
include_if_use(driver_ili9341.MIMXRT1011)
include_if_use(driver_iomuxc.MIMXRT1011)
include_if_use(driver_kpp.MIMXRT1011)
include_if_use(driver_lpi2c.MIMXRT1011)
include_if_use(driver_lpi2c_edma.MIMXRT1011)
include_if_use(driver_lpi2c_freertos.MIMXRT1011)
include_if_use(driver_lpspi.MIMXRT1011)
include_if_use(driver_lpspi_edma.MIMXRT1011)
include_if_use(driver_lpspi_freertos.MIMXRT1011)
include_if_use(driver_lpuart.MIMXRT1011)
include_if_use(driver_lpuart_edma.MIMXRT1011)
include_if_use(driver_lpuart_freertos.MIMXRT1011)
include_if_use(driver_mma8451q.MIMXRT1011)
include_if_use(driver_mx25r_flash.MIMXRT1011)
include_if_use(driver_nand_flash-common.MIMXRT1011)
include_if_use(driver_nand_flash-controller-flexspi.MIMXRT1011)
include_if_use(driver_nic301.MIMXRT1011)
include_if_use(driver_nor_flash-common.MIMXRT1011)
include_if_use(driver_nor_flash-controller-flexspi.MIMXRT1011)
include_if_use(driver_nor_flash-controller-lpspi.MIMXRT1011)
include_if_use(driver_ocotp.MIMXRT1011)
include_if_use(driver_otfad.MIMXRT1011)
include_if_use(driver_pf1550.MIMXRT1011)
include_if_use(driver_pf3000.MIMXRT1011)
include_if_use(driver_pit.MIMXRT1011)
include_if_use(driver_pmu.MIMXRT1011)
include_if_use(driver_pwm.MIMXRT1011)
include_if_use(driver_rtt.MIMXRT1011)
include_if_use(driver_rtt_template.MIMXRT1011)
include_if_use(driver_rtwdog.MIMXRT1011)
include_if_use(driver_sai.MIMXRT1011)
include_if_use(driver_sai_edma.MIMXRT1011)
include_if_use(driver_sgtl5000.MIMXRT1011)
include_if_use(driver_snvs_hp.MIMXRT1011)
include_if_use(driver_snvs_lp.MIMXRT1011)
include_if_use(driver_soc_flexram_allocate.MIMXRT1011)
include_if_use(driver_spdif.MIMXRT1011)
include_if_use(driver_spdif_edma.MIMXRT1011)
include_if_use(driver_src.MIMXRT1011)
include_if_use(driver_tempmon.MIMXRT1011)
include_if_use(driver_trng.MIMXRT1011)
include_if_use(driver_video-common.MIMXRT1011)
include_if_use(driver_wdog01.MIMXRT1011)
include_if_use(driver_wm8904.MIMXRT1011)
include_if_use(driver_wm8960.MIMXRT1011)
include_if_use(driver_xbara.MIMXRT1011)
include_if_use(driver_xip_board_evkmimxrt1010)
include_if_use(driver_xip_device.MIMXRT1011)
include_if_use(middleware_azure_rtos_azure_iot)
include_if_use(middleware_azure_rtos_fx)
include_if_use(middleware_azure_rtos_fx_lib)
include_if_use(middleware_azure_rtos_fx_sp)
include_if_use(middleware_azure_rtos_fx_template)
include_if_use(middleware_azure_rtos_gx)
include_if_use(middleware_azure_rtos_gx_lib)
include_if_use(middleware_azure_rtos_lx)
include_if_use(middleware_azure_rtos_nxd)
include_if_use(middleware_azure_rtos_nxd_lib)
include_if_use(middleware_azure_rtos_nxd_sp)
include_if_use(middleware_azure_rtos_nxd_template)
include_if_use(middleware_azure_rtos_tx)
include_if_use(middleware_azure_rtos_tx_lib)
include_if_use(middleware_azure_rtos_tx_mgr)
include_if_use(middleware_azure_rtos_tx_mgr_lib)
include_if_use(middleware_azure_rtos_tx_mgr_template)
include_if_use(middleware_azure_rtos_tx_sp)
include_if_use(middleware_azure_rtos_tx_template)
include_if_use(middleware_azure_rtos_txm_lib)
include_if_use(middleware_azure_rtos_ux)
include_if_use(middleware_azure_rtos_ux_dci)
include_if_use(middleware_azure_rtos_ux_ehci)
include_if_use(middleware_azure_rtos_ux_ip3511)
include_if_use(middleware_azure_rtos_ux_ip3516)
include_if_use(middleware_azure_rtos_ux_lib)
include_if_use(middleware_azure_rtos_ux_ohci)
include_if_use(middleware_azure_rtos_ux_sp)
include_if_use(middleware_azure_rtos_ux_template)
include_if_use(middleware_baremetal.MIMXRT1011)
include_if_use(middleware_embedded_wizard_evkmimxrt1010)
include_if_use(middleware_embedded_wizard_lib_evkmimxrt10xx)
include_if_use(middleware_embedded_wizard_template_evkmimxrt1010)
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
include_if_use(middleware_fmstr_all_files)
include_if_use(middleware_fmstr_npw_cfg_gen32le)
include_if_use(middleware_fmstr_platform_gen32le)
include_if_use(middleware_freertos-kernel)
include_if_use(middleware_freertos-kernel_extension)
include_if_use(middleware_freertos-kernel_heap_1)
include_if_use(middleware_freertos-kernel_heap_2)
include_if_use(middleware_freertos-kernel_heap_3)
include_if_use(middleware_freertos-kernel_heap_4)
include_if_use(middleware_freertos-kernel_heap_5)
include_if_use(middleware_freertos-kernel_mpu_wrappers)
include_if_use(middleware_freertos-kernel_template)
include_if_use(middleware_issdk_drivers_gpio_imx)
include_if_use(middleware_issdk_drivers_gpio_imx8)
include_if_use(middleware_issdk_drivers_gpio_imxrt)
include_if_use(middleware_issdk_drivers_gpio_imxrt1180)
include_if_use(middleware_issdk_drivers_gpio_imxrt600)
include_if_use(middleware_issdk_drivers_host)
include_if_use(middleware_issdk_drivers_systick_utils)
include_if_use(middleware_issdk_sensor_allregdefs)
include_if_use(middleware_issdk_sensor_fxas21002)
include_if_use(middleware_issdk_sensor_fxlc95000)
include_if_use(middleware_issdk_sensor_fxls8471q)
include_if_use(middleware_issdk_sensor_fxls8961af)
include_if_use(middleware_issdk_sensor_fxls8962)
include_if_use(middleware_issdk_sensor_fxls896xaf)
include_if_use(middleware_issdk_sensor_fxls8971cf)
include_if_use(middleware_issdk_sensor_fxls8974cf)
include_if_use(middleware_issdk_sensor_fxos8700)
include_if_use(middleware_issdk_sensor_fxpq3115)
include_if_use(middleware_issdk_sensor_fxps7250d4)
include_if_use(middleware_issdk_sensor_interface_common)
include_if_use(middleware_issdk_sensor_isl29023)
include_if_use(middleware_issdk_sensor_mag3110)
include_if_use(middleware_issdk_sensor_mma845x)
include_if_use(middleware_issdk_sensor_mma8491q)
include_if_use(middleware_issdk_sensor_mma865x)
include_if_use(middleware_issdk_sensor_mma9553)
include_if_use(middleware_issdk_sensor_mpl3115)
include_if_use(middleware_issdk_sensor_nmh1000)
include_if_use(middleware_issdk_sensor_nps300x)
include_if_use(middleware_issdk_sensor_tools)
include_if_use(middleware_libjpeg)
include_if_use(middleware_libjpeg_template)
include_if_use(middleware_llhttp)
include_if_use(middleware_lvgl)
include_if_use(middleware_lvgl_demo_benchmark)
include_if_use(middleware_lvgl_demo_stress)
include_if_use(middleware_lvgl_demo_widgets)
include_if_use(middleware_lvgl_template)
include_if_use(middleware_lvgl_unused_files)
include_if_use(middleware_mbedtls)
include_if_use(middleware_mbedtls_els_pkc_config)
include_if_use(middleware_mbedtls_port_ksdk)
include_if_use(middleware_mbedtls_template)
include_if_use(middleware_mcu-boot_CMSIS_cm7_headers)
include_if_use(middleware_mcu-boot_MIMXRT1011_drivers)
include_if_use(middleware_mcu-boot_MIMXRT1011_sources)
include_if_use(middleware_mcu-boot_MIMXRT1011_startup)
include_if_use(middleware_mcu-boot_bootsources)
include_if_use(middleware_mcu-boot_drv_edgelock)
include_if_use(middleware_mcu-boot_drv_flexspi_nand)
include_if_use(middleware_mcu-boot_drv_microseconds)
include_if_use(middleware_mcu-boot_drv_microseconds_lpit)
include_if_use(middleware_mcu-boot_drv_microseconds_pit)
include_if_use(middleware_mcu-boot_drv_ocotp)
include_if_use(middleware_mcu-boot_imxrt1010_files)
include_if_use(middleware_mcu-boot_imxrt_files)
include_if_use(middleware_mcu-boot_mem_spi_nand)
include_if_use(middleware_mcu-boot_sdphost)
include_if_use(middleware_mcu-boot_src_bm_usb_hid)
include_if_use(middleware_mcu-boot_src_startup)
include_if_use(middleware_netxduo_imxrt)
include_if_use(middleware_pkcs11)
include_if_use(middleware_tinycbor)
include_if_use(middleware_usb_common_header)
include_if_use(middleware_usb_device_audio_external)
include_if_use(middleware_usb_device_ccid_external)
include_if_use(middleware_usb_device_cdc_external)
include_if_use(middleware_usb_device_cdc_rndis_external)
include_if_use(middleware_usb_device_common_header)
include_if_use(middleware_usb_device_controller_driver)
include_if_use(middleware_usb_device_dfu_external)
include_if_use(middleware_usb_device_ehci)
include_if_use(middleware_usb_device_ehci_config_header)
include_if_use(middleware_usb_device_hid_external)
include_if_use(middleware_usb_device_msd_external)
include_if_use(middleware_usb_device_phdc_external)
include_if_use(middleware_usb_device_printer_external)
include_if_use(middleware_usb_device_stack_external)
include_if_use(middleware_usb_device_video_external)
include_if_use(middleware_usb_host_audio)
include_if_use(middleware_usb_host_cdc)
include_if_use(middleware_usb_host_cdc_rndis)
include_if_use(middleware_usb_host_common_header)
include_if_use(middleware_usb_host_ehci)
include_if_use(middleware_usb_host_ehci_config_header)
include_if_use(middleware_usb_host_hid)
include_if_use(middleware_usb_host_msd)
include_if_use(middleware_usb_host_phdc)
include_if_use(middleware_usb_host_printer)
include_if_use(middleware_usb_host_stack)
include_if_use(middleware_usb_host_video)
include_if_use(middleware_usb_phy)
include_if_use(utilities_misc_utilities.MIMXRT1011)
include_if_use(utility_assert.MIMXRT1011)
include_if_use(utility_assert_lite.MIMXRT1011)
include_if_use(utility_debug_console.MIMXRT1011)
include_if_use(utility_debug_console_lite.MIMXRT1011)
include_if_use(utility_notifier.MIMXRT1011)
include_if_use(utility_shell.MIMXRT1011)
include_if_use(utility_str.MIMXRT1011)
