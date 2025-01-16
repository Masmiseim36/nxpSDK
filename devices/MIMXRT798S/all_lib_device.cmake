# Copy variable into project config.cmake to use software component
#set.board.mimxrt700evk
#  # description: flash config block
#  set(CONFIG_USE_driver_flash_config true)

#  # description: Board_project_template mimxrt700evk
#  set(CONFIG_USE_board_project_template true)

#set.kit.mimxrt700evk_a8974.issdk
#  # description: BOARD_Project_Template mimxrt700evk_a8974
#  set(CONFIG_USE_kit_project_template_mimxrt700evk_a8974 true)

#set.device.MIMXRT798S
#  # description: Utilities which is needed for particular toolchain like the SBRK function required to address limitation between HEAP and STACK in GCC toolchain library.
#  set(CONFIG_USE_utilities_misc_utilities true)

#  # description: Used to include slave core binary into master core binary.
#  set(CONFIG_USE_utility_incbin true)

#  # description: Utility debug_console Template Config
#  set(CONFIG_USE_utility_debug_console_template_config true)

#  # description: common Driver
#  set(CONFIG_USE_driver_common true)

#  # description: SOC MIPI DSI Driver
#  set(CONFIG_USE_driver_soc_mipi_dsi true)

#  # description: Clock Driver
#  set(CONFIG_USE_driver_clock true)

#  # description: EDMA SOC Driver
#  set(CONFIG_USE_driver_edma_soc true)

#  # description: Inputmux_connections Driver
#  set(CONFIG_USE_driver_inputmux_connections true)

#  # description: iopctl Driver
#  set(CONFIG_USE_driver_iopctl_soc true)

#  # description: Reset Driver
#  set(CONFIG_USE_driver_reset true)

#  # description: Wi-Fi and BT module configs
#  set(CONFIG_USE_component_wifi_bt_module_config true)

#  # description: Wi-Fi module Tx power limits
#  set(CONFIG_USE_component_wifi_bt_module_tx_pwr_limits true)

#  # description: Simple Unit Testing for C
#  set(CONFIG_USE_utilities_unity true)

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

#  # description: Rte_device
#  set(CONFIG_USE_device_RTE true)

#  # description: Devices_project_template MIMXRT798S
#  set(CONFIG_USE_device_project_template true)

#  # description: UTICK Driver
#  set(CONFIG_USE_driver_utick true)

#  # description: DSP Driver
#  set(CONFIG_USE_driver_dsp true)

#  # description: Utility debug_console_lite
#  set(CONFIG_USE_utility_debug_console_lite true)

#  # description: Utility str
#  set(CONFIG_USE_utility_str true)

#  # description: Utility debug_console
#  set(CONFIG_USE_utility_debug_console true)

#  # description: Utility assert
#  set(CONFIG_USE_utility_assert true)

#  # description: Utility assert_lite
#  set(CONFIG_USE_utility_assert_lite true)

#  # description: Device MIMXRT798S_startup
#  set(CONFIG_USE_device_startup true)

#  # description: LPFLEXCOMM LPUART FreeRTOS Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpuart_freertos true)

#  # description: LPFLEXCOMM LPSPI FreeRTOS Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpspi_freertos true)

#  # description: LPFLEXCOMM LPI2C FreeRTOS Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpi2c_freertos true)

#  # description: XSPI Driver
#  set(CONFIG_USE_driver_xspi true)

#  # description: XSPI EDMA Driver
#  set(CONFIG_USE_driver_xspi_edma true)

#  # description: WWDT Driver
#  set(CONFIG_USE_driver_wwdt true)

#  # description: USDHC Driver
#  set(CONFIG_USE_driver_usdhc true)

#  # description: TRNG Driver
#  set(CONFIG_USE_driver_trng true)

#  # description: SYSPM Driver
#  set(CONFIG_USE_driver_syspm true)

#  # description: SEMA42 Driver
#  set(CONFIG_USE_driver_sema42 true)

#  # description: SDADC Driver
#  set(CONFIG_USE_driver_sdadc true)

#  # description: SCT Driver
#  set(CONFIG_USE_driver_sctimer true)

#  # description: SAI Driver
#  set(CONFIG_USE_driver_sai true)

#  # description: SAI EDMA Driver
#  set(CONFIG_USE_driver_sai_edma true)

#  # description: PUFv3 Driver
#  set(CONFIG_USE_driver_puf_v3 true)

#  # description: PNGDEC Driver
#  set(CONFIG_USE_driver_pngdec true)

#  # description: PINT Driver
#  set(CONFIG_USE_driver_pint true)

#  # description: PDM Driver
#  set(CONFIG_USE_driver_pdm true)

#  # description: PDM EDMA Driver
#  set(CONFIG_USE_driver_pdm_edma true)

#  # description: OSTimer Driver
#  set(CONFIG_USE_driver_ostimer true)

#  # description: MU Driver
#  set(CONFIG_USE_driver_mu1 true)

#  # description: MRT Driver
#  set(CONFIG_USE_driver_mrt true)

#  # description: MMU Driver
#  set(CONFIG_USE_driver_mmu true)

#  # description: MIPI DSI Driver
#  set(CONFIG_USE_driver_mipi_dsi true)

#  # description: LPUART CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpuart true)

#  # description: LPSPI CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpspi true)

#  # description: LPI2C CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_lpi2c true)

#  # description: LPUART Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpuart true)

#  # description: LPUART EDMA Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpuart_edma true)

#  # description: LPSPI Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpspi true)

#  # description: LPSPI EDMA Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpspi_edma true)

#  # description: LPI2C Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpi2c true)

#  # description: LPI2C EDMA Driver
#  set(CONFIG_USE_driver_lpflexcomm_lpi2c_edma true)

#  # description: LPFLEXCOMM Driver
#  set(CONFIG_USE_driver_lpflexcomm true)

#  # description: lpc_freqme Driver
#  set(CONFIG_USE_driver_lpc_freqme true)

#  # description: LPADC Driver
#  set(CONFIG_USE_driver_lpadc true)

#  # description: LCDIF Driver
#  set(CONFIG_USE_driver_lcdif true)

#  # description: JPEG decoder Driver
#  set(CONFIG_USE_driver_jpegdec true)

#  # description: itrc Driver
#  set(CONFIG_USE_driver_itrc true)

#  # description: IRTC Driver
#  set(CONFIG_USE_driver_irtc true)

#  # description: INPUTMUX Driver
#  set(CONFIG_USE_driver_inputmux true)

#  # description: I3C Driver
#  set(CONFIG_USE_driver_i3c true)

#  # description: I3C EDMA Driver
#  set(CONFIG_USE_driver_i3c_edma true)

#  # description: GPIO Driver
#  set(CONFIG_USE_driver_gpio true)

#  # description: GLIKEY Driver
#  set(CONFIG_USE_driver_glikey true)

#  # description: GDET Driver
#  set(CONFIG_USE_driver_gdet true)

#  # description: FLEXIO Driver
#  set(CONFIG_USE_driver_flexio_camera true)

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

#  # description: EDMA Driver
#  set(CONFIG_USE_driver_edma4 true)

#  # description: CTimer Driver
#  set(CONFIG_USE_driver_ctimer true)

#  # description: CRC Driver
#  set(CONFIG_USE_driver_crc true)

#  # description: cdog Driver
#  set(CONFIG_USE_driver_cdog true)

#  # description: CACHE XCACHE Driver
#  set(CONFIG_USE_driver_cache_xcache true)

#  # description: ACMP Driver
#  set(CONFIG_USE_driver_acmp true)

#  # description: EZHV Driver
#  set(CONFIG_USE_driver_ezhv true)

#  # description: Power driver
#  set(CONFIG_USE_driver_power true)

#  # description: ROMAPI Driver
#  set(CONFIG_USE_driver_romapi true)

#  # description: Driver camera-common
#  set(CONFIG_USE_driver_camera-common true)

#  # description: Driver camera-device-common
#  set(CONFIG_USE_driver_camera-device-common true)

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

#  # description: Driver dbi
#  set(CONFIG_USE_driver_dbi true)

#  # description: Driver dbi_flexio_edma
#  set(CONFIG_USE_driver_dbi_flexio_edma true)

#  # description: Driver dbi_lcdif
#  set(CONFIG_USE_driver_dbi_lcdif true)

#  # description: Driver dc-fb-common
#  set(CONFIG_USE_driver_dc-fb-common true)

#  # description: Driver dc-fb-dbi
#  set(CONFIG_USE_driver_dc-fb-dbi true)

#  # description: Driver dc-fb-dsi-cmd
#  set(CONFIG_USE_driver_dc-fb-dsi-cmd true)

#  # description: Driver dc-fb-lcdif
#  set(CONFIG_USE_driver_dc-fb-lcdif true)

#  # description: Driver dc-fb-ssd1963
#  set(CONFIG_USE_driver_dc-fb-ssd1963 true)

#  # description: Driver display-common
#  set(CONFIG_USE_driver_display-common true)

#  # description: Driver display-hx8394
#  set(CONFIG_USE_driver_display-hx8394 true)

#  # description: Driver display-mipi-dsi-cmd
#  set(CONFIG_USE_driver_display-mipi-dsi-cmd true)

#  # description: Driver display-rm67162
#  set(CONFIG_USE_driver_display-rm67162 true)

#  # description: Driver display-co5300
#  set(CONFIG_USE_driver_display-co5300 true)

#  # description: Driver display-rm67191
#  set(CONFIG_USE_driver_display-rm67191 true)

#  # description: Driver display-rm68191
#  set(CONFIG_USE_driver_display-rm68191 true)

#  # description: Driver display-rm68200
#  set(CONFIG_USE_driver_display-rm68200 true)

#  # description: Driver display-rm692c9
#  set(CONFIG_USE_driver_display-rm692c9 true)

#  # description: Driver display-rpi
#  set(CONFIG_USE_driver_display-rpi true)

#  # description: Driver fbdev
#  set(CONFIG_USE_driver_fbdev true)

#  # description: Driver video-common
#  set(CONFIG_USE_driver_video-common true)

#  # description: Driver ili9341
#  set(CONFIG_USE_driver_ili9341 true)

#  # description: Driver psp27801
#  set(CONFIG_USE_driver_psp27801 true)

#  # description: Driver ssd1963
#  set(CONFIG_USE_driver_ssd1963 true)

#  # description: Driver st7796s
#  set(CONFIG_USE_driver_st7796s true)

#  # description: Driver ft5406_rt
#  set(CONFIG_USE_driver_ft5406_rt true)

#  # description: Driver ft6x06
#  set(CONFIG_USE_driver_ft6x06 true)

#  # description: Component lpuart_adapter
#  set(CONFIG_USE_component_lpuart_adapter true)

#  # description: Component timer_manager
#  set(CONFIG_USE_component_timer_manager true)

#  # description: Component ctimer_adapter
#  set(CONFIG_USE_component_ctimer_adapter true)

#  # description: Component mrt_adapter
#  set(CONFIG_USE_component_mrt_adapter true)

#  # description: Component ostimer_adapter
#  set(CONFIG_USE_component_ostimer_adapter true)

#  # description: Utility shell
#  set(CONFIG_USE_utility_shell true)

#  # description: Component serial_manager
#  set(CONFIG_USE_component_serial_manager true)

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

#  # description: Component power manager core level
#  set(CONFIG_USE_component_power_manager_framework true)

#  # description: Driver pca9420
#  set(CONFIG_USE_driver_pca9420 true)

#  # description: Driver pca9422
#  set(CONFIG_USE_driver_pca9422 true)

#  # description: Driver pf1550
#  set(CONFIG_USE_driver_pf1550 true)

#  # description: Driver pf3000
#  set(CONFIG_USE_driver_pf3000 true)

#  # description: Driver pf5020
#  set(CONFIG_USE_driver_pf5020 true)

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

#  # description: Component i3c_bus
#  set(CONFIG_USE_component_i3c_bus true)

#  # description: Component i3c_bus_adapter
#  set(CONFIG_USE_component_i3c_bus_adapter true)

#  # description: Component i3c_adapter
#  set(CONFIG_USE_component_i3c_adapter true)

#  # description: Component lpi2c_adapter
#  set(CONFIG_USE_component_lpi2c_adapter true)

#  # description: Component i2c_adapter_interface
#  set(CONFIG_USE_component_i2c_adapter_interface true)

#  # description: Component gpio_adapter
#  set(CONFIG_USE_component_gpio_adapter true)

#  # description: Component flash_nor_xspi
#  set(CONFIG_USE_component_flash_nor_xspi true)

#  # description: mflash onchip
#  set(CONFIG_USE_component_mflash_onchip true)

#  # description: mflash offchip
#  set(CONFIG_USE_component_mflash_offchip true)

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

#  # description: Device MIMXRT798S_cmsis
#  set(CONFIG_USE_device_CMSIS true)

#  # description: Device MIMXRT798S_system
#  set(CONFIG_USE_device_system true)

#  # description: Core headers for RISCV cores
#  set(CONFIG_USE_RISCV_Include_Core true)

#set.middleware.issdk
#  # description: ISSDK Utils
#  set(CONFIG_USE_middleware_issdk_drivers_systick_utils true)

#  # description: ISSDK Sensor Interface Common
#  set(CONFIG_USE_middleware_issdk_sensor_interface_common true)

#  # description: ISSDK Allregdefs
#  set(CONFIG_USE_middleware_issdk_sensor_allregdefs true)

#  # description: ISSDK GPIO Driver for MIMXRT798S
#  set(CONFIG_USE_middleware_issdk_drivers_gpio_imxrt700 true)

#  # description: ISSDK FXLS8974CF Sensor Driver Files
#  set(CONFIG_USE_middleware_issdk_sensor_fxls8974cf true)

#set.middleware.freertos-aws_iot
#  # description: Client implementation of the MQTT 3.1.1 specification for embedded devices.
#  set(CONFIG_USE_middleware_freertos_coremqtt true)

#  # description: A parser strictly enforcing the ECMA-404 JSON standard, suitable for microcontrollers.
#  set(CONFIG_USE_middleware_freertos_corejson true)

#  # description: Algorithm for calculating exponential backoff with jitter for network retry attempts.
#  set(CONFIG_USE_middleware_freertos_backoffalgorithm true)

#  # description: Client library for using the AWS IoT Device Defender service on embedded devices.
#  set(CONFIG_USE_middleware_aws_iot_device_defender true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_freertos_corepkcs11_template true)

#  # description: Software implementation of the PKCS #11 standard.
#  set(CONFIG_USE_middleware_freertos_corepkcs11 true)

#  # description: PKCS #11, mbedtls port.
#  set(CONFIG_USE_middleware_freertos_corepkcs11_mbedtls true)

#  # description: PKCS #11, mbedtls_utils
#  set(CONFIG_USE_middleware_freertos_corepkcs11_mbedtls_utils true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_freertos_coremqtt_template true)

#  # description: Agent for thread-safe use of coreMQTT.
#  set(CONFIG_USE_middleware_freertos_coremqtt-agent true)

#  # description: Client library for using the AWS Over-the-air Update service on embedded devices.
#  set(CONFIG_USE_middleware_aws_iot_ota true)

#  # description: Client library for using the AWS Over-the-air Update service on embedded devices. FreeRTOS port.
#  set(CONFIG_USE_middleware_aws_iot_ota_freertos true)

#  # description: Library for using the AWS IoT Jobs service on embedded devices.
#  set(CONFIG_USE_middleware_aws_iot_jobs true)

#  # description: kvstore, IoT reference common example component
#  set(CONFIG_USE_middleware_iot_reference_kvstore true)

#  # description: logging, IoT reference common example component
#  set(CONFIG_USE_middleware_iot_reference_logging true)

#  # description: mqtt_agent, IoT reference common example component
#  set(CONFIG_USE_middleware_iot_reference_mqtt_agent true)

#  # description: mqtt-agent-interface, IoT reference common example component
#  set(CONFIG_USE_middleware_iot_reference_mqtt_agent_interface true)

#  # description: transport_mbedtls3x, IoT reference common example component
#  set(CONFIG_USE_middleware_iot_reference_transport_mbedtls3x true)

#  # description: transport_mbedtls_psa, IoT reference common example component
#  set(CONFIG_USE_middleware_iot_reference_transport_mbedtls_psa true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_aws_iot_device_shadow_template true)

#  # description: Client library for using the AWS IoT Device Shadow service on embedded devices.
#  set(CONFIG_USE_middleware_aws_iot_device_shadow true)

#set.middleware.wifi
#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_wifi_wifi_bt_config_template true)

#  # description: NXP WLAN OSA
#  set(CONFIG_USE_middleware_wifi_osa_free_rtos true)

#  # description: NXP WLAN OSA
#  set(CONFIG_USE_middleware_wifi_osa_thread true)

#  # description: NXP WLAN f/w dnld base
#  set(CONFIG_USE_middleware_wifi_firmware_download true)

#  # description: NXP WLAN OSA
#  set(CONFIG_USE_middleware_wifi_net_thread true)

#  # description: NXP Wi-Fi Interface Abstraction Layer
#  set(CONFIG_USE_middleware_wifi_fwdnld_intf_abs true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_wifi_template true)

#  # description: NXP WLAN OSA
#  set(CONFIG_USE_middleware_wifi_osa true)

#  # description: NXP WLAN common files
#  set(CONFIG_USE_middleware_wifi_common_files true)

#  # description: NXP WLAN f/w dnld driver
#  set(CONFIG_USE_middleware_wifi_fwdnld true)

#  # description: NXP WLAN OSA
#  set(CONFIG_USE_middleware_wifi_net_free_rtos true)

#  # description: NXP WLAN OSA
#  set(CONFIG_USE_middleware_wifi_net true)

#  # description: NXP Wi-Fi driver
#  set(CONFIG_USE_middleware_wifi_wifidriver true)

#  # description: NXP Wi-Fi driver Soft AP
#  set(CONFIG_USE_middleware_wifi_wifidriver_softap true)

#  # description: NXP Wi-Fi functionality enables customers to quickly develop applications of interest to add connectivity to different sensors and appliances
#  set(CONFIG_USE_middleware_wifi true)

#  # description: NXP Wi-Fi SDIO driver
#  set(CONFIG_USE_middleware_wifi_sdio true)

#  # description: NXP Wi-Fi SDIO driver
#  set(CONFIG_USE_middleware_wifi_mlan_sdio true)

#  # description: Middlware Wi-Fi CLI
#  set(CONFIG_USE_middleware_wifi_cli true)

#  # description: Middlware Wi-Fi WMCRYPTO
#  set(CONFIG_USE_middleware_wifi_wmcrypto true)

#  # description: Middlware Wi-Fi WMTIME
#  set(CONFIG_USE_middleware_wifi_wmtime true)

#  # description: Middlware Wi-Fi NCP_SUPP_WMCRYPTO
#  set(CONFIG_USE_middleware_wifi_ncp_supp_wmcrypto true)

#  # description: Middlware Wi-Fi Location Service
#  set(CONFIG_USE_middleware_wifi_wls true)

#  # description: Edgefast Wi-Fi NXP is a blocking layer for Wi-Fi NXP
#  set(CONFIG_USE_middleware_edgefast_wifi_nxp true)

#set.middleware.vglite
#  # description: vglite library
#  set(CONFIG_USE_middleware_vglite true)

#set.middleware.tinycbor
#  # description: Concise Binary Object Representation (CBOR) Library
#  set(CONFIG_USE_middleware_tinycbor true)

#set.middleware.psatest
#  # description: PSA Test Suite Crypto Non Secure
#  set(CONFIG_USE_middleware_psatest_ns_crypto true)

#  # description: PSA Test Suite Initial Attestation Non Secure part 1
#  set(CONFIG_USE_middleware_psatest_ns_t_cose true)

#  # description: PSA Test Suite Protected Storage Non Secure
#  set(CONFIG_USE_middleware_psatest_ns_protected_storage true)

#  # description: PSA Test Suite Internal Trusted Storage Non Secure
#  set(CONFIG_USE_middleware_psatest_ns_internal_trusted_storage true)

#  # description: PSA Test Suite Non Secure
#  set(CONFIG_USE_middleware_psatest_ns true)

#  # description: PSA Test Suite Non Secure target dev_apis
#  set(CONFIG_USE_middleware_psatest_ns_tgt_dev_apis_nxp true)

#set.middleware.sdmmc
#  # description: Middleware sdmmc common
#  set(CONFIG_USE_middleware_sdmmc_common true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_sdmmc_sdhc_template true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_sdmmc_sdif_template true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_sdmmc_usdhc_template true)

#  # description: Middleware sdmmc osa
#  set(CONFIG_USE_middleware_sdmmc_osa true)

#  # description: Middleware sdmmc sd
#  set(CONFIG_USE_middleware_sdmmc_sd true)

#  # description: Middleware sdmmc mmc
#  set(CONFIG_USE_middleware_sdmmc_mmc true)

#  # description: Middleware sdmmc sdio
#  set(CONFIG_USE_middleware_sdmmc_sdio true)

#  # description: Middleware sdmmc sdspi
#  set(CONFIG_USE_middleware_sdmmc_sdspi true)

#  # description: Middleware sdmmc host usdhc
#  set(CONFIG_USE_middleware_sdmmc_host_usdhc true)

#  # description: Middleware sdmmc host usdhc non_blocking
#  set(CONFIG_USE_middleware_sdmmc_host_usdhc_non_blocking true)

#  # description: Middleware sdmmc host usdhc blocking
#  set(CONFIG_USE_middleware_sdmmc_host_usdhc_blocking true)

#set.middleware.pkcs11
#  # description: Public Key Cryptography Standard #11 defines generic cryptographic token interface.
#  set(CONFIG_USE_middleware_pkcs11 true)

#set.middleware.multicore
#  # description: MCMgr
#  set(CONFIG_USE_middleware_multicore_mcmgr true)

#  # description: eRPC
#  set(CONFIG_USE_middleware_multicore_erpc true)

#  # description: eRPC
#  set(CONFIG_USE_middleware_multicore_erpc_doc true)

#  # description: RPMsg-Lite BM environment sources
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_bm true)

#  # description: RPMsg-Lite FreeRTOS environment sources
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_freertos true)

#  # description: RPMsg-Lite XOS environment layer sources
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_xos true)

#  # description: RPMsg-Lite QNX environment layer sources
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_qnx true)

#  # description: RPMsg-Lite ThreadX environment layer sources
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_threadx true)

#  # description: RPMsg-Lite Zephyr environment sources
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_zephyr true)

#  # description: RPMsg-Lite
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite true)

#  # description: RPMsg-Lite NS
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_ns true)

#  # description: RPMsg-Lite Queue
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_queue true)

#  # description: RPMsg-Lite for imxrt700 application
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_imxrt700 true)

#  # description: RPMsg-Lite for imxrt700 hifi1 application
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_imxrt700_hifi1 true)

#  # description: RPMsg-Lite for imxrt700 hifi4 application
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_imxrt700_hifi4 true)

#  # description: Multicore Manager for imxrt700 board
#  set(CONFIG_USE_middleware_multicore_mcmgr_imxrt700 true)

#  # description: eRPC_client
#  set(CONFIG_USE_middleware_multicore_erpc_client true)

#  # description: eRPC_server
#  set(CONFIG_USE_middleware_multicore_erpc_server true)

#  # description: eRPC_arbitrator
#  set(CONFIG_USE_middleware_multicore_erpc_arbitrator true)

#  # description: eRPC_port_stdlib
#  set(CONFIG_USE_middleware_multicore_erpc_port_stdlib true)

#  # description: eRPC_port_freertos
#  set(CONFIG_USE_middleware_multicore_erpc_port_freertos true)

#  # description: eRPC_mu_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_mu_transport true)

#  # description: eRPC_rpmsg_lite_transport_master
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_rpmsg_lite_transport_master true)

#  # description: eRPC_rpmsg_lite_transport_remote
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_rpmsg_lite_transport_remote true)

#  # description: eRPC_rpmsg_lite_rtos_transport_master
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_rpmsg_lite_rtos_transport_master true)

#  # description: eRPC_rpmsg_lite_rtos_transport_remote
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_rpmsg_lite_rtos_transport_remote true)

#  # description: eRPC_rpmsg_tty_rtos_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_rpmsg_tty_rtos_transport true)

#  # description: eRPC_uart_cmsis_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_uart_cmsis_transport true)

#  # description: eRPC_dspi_master_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_dspi_master_transport true)

#  # description: eRPC_dspi_slave_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_dspi_slave_transport true)

#  # description: eRPC_spi_master_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_spi_master_transport true)

#  # description: eRPC_spi_slave_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_spi_slave_transport true)

#  # description: eRPC_lpspi_slave_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_lpspi_slave_transport true)

#  # description: eRPC_i2c_slave_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_i2c_slave_transport true)

#  # description: eRPC_lpi2c_slave_transport
#  set(CONFIG_USE_middleware_multicore_erpc_eRPC_lpi2c_slave_transport true)

#  # description: eRPC
#  set(CONFIG_USE_middleware_multicore_erpc_examples_erpc_common_multicore true)

#  # description: erpc_examples Matrix Multiply Client
#  set(CONFIG_USE_middleware_multicore_erpc_examples_erpc_matrix_multiply_client true)

#  # description: erpc_examples Matrix Multiply Server
#  set(CONFIG_USE_middleware_multicore_erpc_examples_erpc_matrix_multiply_server true)

#  # description: erpc_examples Two Way RPC core0
#  set(CONFIG_USE_middleware_multicore_erpc_examples_erpc_two_way_rpc_core0 true)

#  # description: erpc_examples Two Way RPC core1
#  set(CONFIG_USE_middleware_multicore_erpc_examples_erpc_two_way_rpc_core1 true)

#set.middleware.mcuboot
#  # description: NXP porting layer for MCUboot's bootutil code
#  set(CONFIG_USE_middleware_mcuboot_nxp_bootutil_port true)

#  # description: MCUboot default stack/heap setup for MbedTLS
#  set(CONFIG_USE_middleware_mcuboot_stack_heap_default_mbedtls true)

#  # description: MCUboot default stack/heap setup for TinyCrypt
#  set(CONFIG_USE_middleware_mcuboot_stack_heap_default_tinycrypt true)

#  # description: MCUboot application support layer
#  set(CONFIG_USE_middleware_mcuboot_nxp_app_support true)

#  # description: Encrypted XIP abstraction for MCUBoot
#  set(CONFIG_USE_middleware_mcuboot_nxp_encrypted_xip true)

#  # description: MCUBoot's subset of MbedTLS for ASN.1 parsing
#  set(CONFIG_USE_middleware_mcuboot_mbedtls-asn1 true)

#  # description: mcuboot
#  set(CONFIG_USE_middleware_mcuboot_bootutil true)

#  # description: MCUboot device flash porting layer
#  set(CONFIG_USE_middleware_mcuboot_bootutil_flashapi_rt true)

#  # description: MCUboot device flash porting layer
#  set(CONFIG_USE_middleware_mcuboot_bootutil_flashapi_mcx true)

#  # description: TinyCrypt fork in MCUBoot repository
#  set(CONFIG_USE_middleware_mcuboot_tinycrypt true)

#set.middleware.mbedtls3x
#  # description: mbedTLS Template with config files.
#  set(CONFIG_USE_middleware_mbedtls3x_template true)

#  # description: mbedTLS port library common files for MCUX
#  set(CONFIG_USE_middleware_mbedtls3x_port_config true)

#  # description: mbedTLS port library common files for MCUX
#  set(CONFIG_USE_middleware_mbedtls3x_port_psa_crypto_config true)

#  # description: mbedTLS port library for hw initialization of trng
#  set(CONFIG_USE_middleware_mbedtls3x_port_hw_init true)

#  # description: mbedTLS test suite
#  set(CONFIG_USE_middleware_mbedtls3x_tests true)

#  # description: mbedTLS crypto library without PSA
#  set(CONFIG_USE_middleware_mbedtls3x_crypto_no_psa true)

#  # description: mbedTLS X.509 library
#  set(CONFIG_USE_middleware_mbedtls3x_x509 true)

#  # description: PSA ITS simulator over stdio files.
#  set(CONFIG_USE_middleware_mbedtls3x_crypto_storage_default true)

#  # description: PSA ITS simulator over objects in RAM.
#  set(CONFIG_USE_middleware_mbedtls3x_crypto_storage_ram true)

#  # description: mbedTLS crypto library
#  set(CONFIG_USE_middleware_mbedtls3x_crypto true)

#  # description: mbedTLS SSL library without PSA
#  set(CONFIG_USE_middleware_mbedtls3x_ssl_no_psa true)

#  # description: mbedTLS SSL library
#  set(CONFIG_USE_middleware_mbedtls3x_ssl true)

#  # description: mbedTLS library v3.x without PSA
#  set(CONFIG_USE_middleware_mbedtls3x_no_psa true)

#  # description: mbedTLS library v3.x
#  set(CONFIG_USE_middleware_mbedtls3x true)

#set.middleware.eiq
#  # description: TensorFlow Lite Micro library
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro true)

#  # description: TensorFlow Lite Micro library binary with core specific kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_refenerce true)

#  # description: Multimedia Processing Pipelines Library
#  set(CONFIG_USE_middleware_eiq_mpp true)

#  # description: TensorFlow Lite Micro library binary with core specific kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_binary true)

#  # description: TensorFlow Lite Micro library binary with core specific kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_source true)

#  # description: TensorFlow Lite Micro library binary with core specific kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_debug_log true)

#  # description: TensorFlow Lite Micro library binary with core specific kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_library true)

#  # description: TensorFlow Lite Micro library with CMSIS-NN kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_cmsis_nn true)

#  # description: TensorFlow Lite Micro word detection library
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_examples_microspeech true)

#  # description: TensorFlow Lite Micro library header files
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_headers true)

#  # description: Neutron kernel implementations for TensorFlow Lite Micro
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_neutron true)

#  # description: TensorFlow Lite Micro library with reference kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_reference true)

#  # description: CMSIS-NN library
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn true)

#  # description: Neutron library binary
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_neutron_lib_binary true)

#  # description: Xtensa HiFi4 NN library
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4 true)

#  # description: Xtensa HiFi4 NN library binary
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4_binary true)

#  # description: TensorFlow Lite Micro library with Xtensa HiFi4 DSP kernel implementations
#  set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_xtensa true)

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

#set.middleware.littlefs
#  # description: littlefs
#  set(CONFIG_USE_middleware_littlefs true)

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

#  # description: FatFs_MMC
#  set(CONFIG_USE_middleware_fatfs_mmc true)

#  # description: FatFs_RAM
#  set(CONFIG_USE_middleware_fatfs_ram true)

#  # description: FatFs_SD
#  set(CONFIG_USE_middleware_fatfs_sd true)

#  # description: FatFs_SDSPI
#  set(CONFIG_USE_middleware_fatfs_sdspi true)

#  # description: FatFs_USB
#  set(CONFIG_USE_middleware_fatfs_usb true)

#set.middleware.emwin
#  # description: emwin template
#  set(CONFIG_USE_middleware_emwin_template true)

#  # description: emWin library
#  set(CONFIG_USE_middleware_emwin true)

#set.middleware.cadence.nnlib
#  # description: DSP Neural Networks
#  set(CONFIG_USE_middleware_cadence_nnlib true)

#  # description: DSP Neural Networks
#  set(CONFIG_USE_middleware_cadence_nnlib_rt700_hifi4 true)

#  # description: DSP Neural Networks
#  set(CONFIG_USE_middleware_cadence_nnlib_rt700_hifi1 true)

#set.component.psa_crypto_driver
#  # description: Common files of PSA crypto driver for ele_s200
#  set(CONFIG_USE_component_psa_crypto_driver_ele_s200_common true)

#  # description: Transparent part of PSA crypto driver for ele_s200
#  set(CONFIG_USE_component_psa_crypto_driver_ele_s200_transparent true)

#  # description: Common files of PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc_common true)

#  # description: Opaque part of PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc_opaque true)

#  # description: RW61x Oracle of PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc_oracle_rw61x true)

#  # description: MIMXRT Oracle of PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc_oracle_mimxrt true)

#  # description: MCXN Oracle of PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc_oracle_mcxn true)

#  # description: Transparent part of PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc_transparent true)

#  # description: Threading Alt implementation for mbedtls layer via PSA crypto driver
#  set(CONFIG_USE_component_psa_crypto_driver_mbedtls_thread_alt true)

#  # description: OS Abstraction Layer for PSA crypto driver
#  set(CONFIG_USE_component_psa_crypto_driver_osal true)

#  # description: psa_crypto_driver_trng usage compile option
#  set(CONFIG_USE_component_psa_crypto_driver_use_trng_compile_option true)

#  # description: PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc true)

#  # description: Oracle of PSA crypto driver for els_pkc
#  set(CONFIG_USE_component_psa_crypto_driver_els_pkc_oracle true)

#  # description: OS Abstraction Layer for PSA crypto driver on bare metal
#  set(CONFIG_USE_component_psa_crypto_driver_osal_baremetal true)

#  # description: OS Abstraction Layer for PSA crypto driver on FreeRTOS
#  set(CONFIG_USE_component_psa_crypto_driver_osal_frtos true)

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

#set.component.els_pkc
#  # description: Component els_pkc.core
#  set(CONFIG_USE_component_els_pkc_core true)

#  # description: Component els_pkc.data_integrity
#  set(CONFIG_USE_component_els_pkc_data_integrity true)

#  # description: Component els_pkc.ecc_deterministic
#  set(CONFIG_USE_component_els_pkc_ecc_deterministic true)

#  # description: Component els_header_only
#  set(CONFIG_USE_component_els_pkc_els_header_only true)

#  # description: Component els_pkc.key_derivation
#  set(CONFIG_USE_component_els_pkc_key_derivation true)

#  # description: Component els_pkc.pre_processor
#  set(CONFIG_USE_component_els_pkc_pre_processor true)

#  # description: Component els_pkc.secure_counter
#  set(CONFIG_USE_component_els_pkc_secure_counter true)

#  # description: Component els_pkc toolchain
#  set(CONFIG_USE_component_els_pkc_toolchain true)

#  # description: Component els_pkc of Crypto Lib
#  set(CONFIG_USE_component_els_pkc true)

#  # description: Component els_pkc.aead
#  set(CONFIG_USE_component_els_pkc_aead true)

#  # description: Component els_pkc.aead_modes
#  set(CONFIG_USE_component_els_pkc_aead_modes true)

#  # description: Component aes
#  set(CONFIG_USE_component_els_pkc_aes true)

#  # description: Component buffer
#  set(CONFIG_USE_component_els_pkc_buffer true)

#  # description: Component els_pkc.cipher
#  set(CONFIG_USE_component_els_pkc_cipher true)

#  # description: Component els_pkc.cipher
#  set(CONFIG_USE_component_els_pkc_cipher_modes true)

#  # description: Component els pkc common
#  set(CONFIG_USE_component_els_pkc_common true)

#  # description: Component crc
#  set(CONFIG_USE_component_els_pkc_crc true)

#  # description: Component els_pkc.doc.mimxrt
#  set(CONFIG_USE_component_els_pkc_doc_mimxrt true)

#  # description: Component els_pkc.ecc
#  set(CONFIG_USE_component_els_pkc_ecc true)

#  # description: Component els
#  set(CONFIG_USE_component_els_pkc_els true)

#  # description: Component els_pkc els_common
#  set(CONFIG_USE_component_els_pkc_els_common true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_aead true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_cipher_modes true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_ecc true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_els true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_flow_protection true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_hash_modes true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_hmac true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_key true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_mac_modes true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_memory true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_random_modes true)

#  # description: Component els_pkc_test
#  set(CONFIG_USE_component_els_pkc_examples_rsa true)

#  # description: Component els_pkc.flow_protection
#  set(CONFIG_USE_component_els_pkc_flow_protection true)

#  # description: Component glikey
#  set(CONFIG_USE_component_els_pkc_glikey true)

#  # description: Component els_pkc.hash
#  set(CONFIG_USE_component_els_pkc_hash true)

#  # description: Component els_pkc.hashmodes
#  set(CONFIG_USE_component_els_pkc_hashmodes true)

#  # description: Component els_pkc.hmac
#  set(CONFIG_USE_component_els_pkc_hmac true)

#  # description: Component els_pkc.key
#  set(CONFIG_USE_component_els_pkc_key true)

#  # description: Component els_pkc.mac
#  set(CONFIG_USE_component_els_pkc_mac true)

#  # description: Component els_pkc.mac_modes
#  set(CONFIG_USE_component_els_pkc_mac_modes true)

#  # description: Component els_pkc.math
#  set(CONFIG_USE_component_els_pkc_math true)

#  # description: Component els_pkc.memory
#  set(CONFIG_USE_component_els_pkc_memory true)

#  # description: Component els_pkc.oscca
#  set(CONFIG_USE_component_els_pkc_oscca true)

#  # description: Component els_pkc.oscca_aeadmodes
#  set(CONFIG_USE_component_els_pkc_oscca_aeadmodes true)

#  # description: Component els_pkc.oscca_ciphermodes
#  set(CONFIG_USE_component_els_pkc_oscca_ciphermodes true)

#  # description: Component els_pkc.oscca_macmodes
#  set(CONFIG_USE_component_els_pkc_oscca_macmodes true)

#  # description: Component els_pkc.oscca_pkc
#  set(CONFIG_USE_component_els_pkc_oscca_pkc true)

#  # description: Component els_pkc.oscca_randommodes
#  set(CONFIG_USE_component_els_pkc_oscca_randommodes true)

#  # description: Component els_pkc.oscca_safo
#  set(CONFIG_USE_component_els_pkc_oscca_safo true)

#  # description: Component els_pkc.oscca_sm2
#  set(CONFIG_USE_component_els_pkc_oscca_sm2 true)

#  # description: Component els_pkc.oscca_sm3
#  set(CONFIG_USE_component_els_pkc_oscca_sm3 true)

#  # description: Component els_pkc.oscca_sm4
#  set(CONFIG_USE_component_els_pkc_oscca_sm4 true)

#  # description: Component els_pkc.padding
#  set(CONFIG_USE_component_els_pkc_padding true)

#  # description: Component els_pkc.param_integrity
#  set(CONFIG_USE_component_els_pkc_param_integrity true)

#  # description: Component pkc
#  set(CONFIG_USE_component_els_pkc_pkc true)

#  # description: Component els_pkc_mimxrt
#  set(CONFIG_USE_component_els_pkc_platform_mimxrt true)

#  # description: Component prng
#  set(CONFIG_USE_component_els_pkc_prng true)

#  # description: Component els_pkc_psa_driver
#  set(CONFIG_USE_component_els_pkc_psa_driver true)

#  # description: Component els_pkc.random
#  set(CONFIG_USE_component_els_pkc_random true)

#  # description: Component els_pkc.random_modes
#  set(CONFIG_USE_component_els_pkc_random_modes true)

#  # description: Component els_pkc.random_modes_ctr
#  set(CONFIG_USE_component_els_pkc_random_modes_ctr true)

#  # description: Component els_pkc.random_modes_hmacdrbg
#  set(CONFIG_USE_component_els_pkc_random_modes_hmacdrbg true)

#  # description: Component els_pkc.rsa
#  set(CONFIG_USE_component_els_pkc_rsa true)

#  # description: Component els_pkc.rsa_oaep
#  set(CONFIG_USE_component_els_pkc_rsa_oaep true)

#  # description: Component els_pkc.session
#  set(CONFIG_USE_component_els_pkc_session true)

#  # description: Component els_pkc.signature
#  set(CONFIG_USE_component_els_pkc_signature true)

#  # description: Component els_pkc standalone_gdet
#  set(CONFIG_USE_component_els_pkc_standalone_gdet true)

#  # description: Component els_pkc static_lib MIMXRT
#  set(CONFIG_USE_component_els_pkc_static_lib_mimxrt true)

#  # description: Component els_pkc.trng
#  set(CONFIG_USE_component_els_pkc_trng true)

#  # description: Component els_pkc.trng.type_els
#  set(CONFIG_USE_component_els_pkc_trng_type_els true)

#  # description: Component els_pkc.trng.type_rng4
#  set(CONFIG_USE_component_els_pkc_trng_type_rng4 true)

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

#set.middleware.edgefast_bluetooth
#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_pal true)

#  # description: edgefast_bluetooth HCI
#  set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci true)

#  # description: edgefast_bluetooth HCI UART
#  set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart true)

#  # description: edgefast_bluetooth bt_ble host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind true)

#  # description: edgefast_bluetooth bt_ble host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_lc3 true)

#  # description: edgefast_bluetooth wifi_nxp controller
#  set(CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_base true)

#  # description: edgefast_bluetooth k32w061 controller
#  set(CONFIG_USE_middleware_edgefast_bluetooth_k32w061_controller true)

#  # description: edgefast_bluetooth rw610 controller
#  set(CONFIG_USE_middleware_edgefast_bluetooth_rw610_controller true)

#  # description: edgefast_bluetooth configuration for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_config_ethermind true)

#  # description: edgefast_bluetooth extension for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_extension_common_ethermind true)

#  # description: edgefast_bluetooth bt_ble Host stack(including BR and BLE)
#  set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind true)

#  # description: edgefast_bluetooth bt_ble Host stack(including BLE only)
#  set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind true)

#  # description: edgefast_bluetooth bt_ble Host stack(including BR only)
#  set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind true)

#  # description: edgefast_bluetooth configuration template for LE audio
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_config_template true)

#  # description: edgefast_bluetooth le_audio.cap
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_cap true)

#  # description: edgefast_bluetooth le_audio.aics
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_aics true)

#  # description: edgefast_bluetooth le_audio.vocs
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vocs true)

#  # description: edgefast_bluetooth le_audio.vcs
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vcp true)

#  # description: edgefast_bluetooth le_audio.micp
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_micp true)

#  # description: edgefast_bluetooth le_audio.csis
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_csip true)

#  # description: edgefast_bluetooth le_audio.ccid
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_ccid true)

#  # description: edgefast_bluetooth le_audio.mcs
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mcs true)

#  # description: edgefast_bluetooth le_audio.mcc
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mcc true)

#  # description: edgefast_bluetooth le_audio.mpl
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_mpl true)

#  # description: edgefast_bluetooth le_audio.baps
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_bap true)

#  # description: edgefast_bluetooth le_audio.bass
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_bass true)

#  # description: edgefast_bluetooth le_audio.has
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_has true)

#  # description: edgefast_bluetooth le_audio.tbs
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_tbs true)

#  # description: edgefast_bluetooth le_audio.tmap
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_tmap true)

#  # description: edgefast_bluetooth le_audio.gmap
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_gmap true)

#  # description: edgefast_bluetooth le_audio.pbp
#  set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_pbp true)

#  # description: edgefast_bluetooth profile hrs
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_hrs true)

#  # description: edgefast_bluetooth profile bas
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_bas true)

#  # description: edgefast_bluetooth profile dis
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_dis true)

#  # description: edgefast_bluetooth profile hts
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_hts true)

#  # description: edgefast_bluetooth profile ias
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ias true)

#  # description: edgefast_bluetooth profile hps
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_hps true)

#  # description: edgefast_bluetooth profile pxr
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_pxr true)

#  # description: edgefast_bluetooth profile ipsp
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ipsp true)

#  # description: edgefast_bluetooth profile ots
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ots true)

#  # description: edgefast_bluetooth profile wu
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_wu true)

#  # description: edgefast_bluetooth profile fmp
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_fmp true)

#  # description: edgefast_bluetooth profile tip
#  set(CONFIG_USE_middleware_edgefast_bluetooth_profile_tip true)

#  # description: edgefast_bluetooth PAL crypto support for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_pal_crypto_ethermind true)

#  # description: edgefast_bluetooth PAL platform for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_pal_platform_ethermind true)

#  # description: edgefast_bluetooth host_msd_fatfs for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind true)

#  # description: edgefast_bluetooth db_gen for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_pal_db_gen_ethermind true)

#  # description: edgefast_bluetooth porting.atomic
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_atomic true)

#  # description: edgefast_bluetooth porting.list
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_list true)

#  # description: edgefast_bluetooth porting.netbuf
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_net true)

#  # description: edgefast_bluetooth porting.toolchain
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_toolchain true)

#  # description: edgefast_bluetooth porting.work_queue
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_work_queue true)

#  # description: Template configuration file to be edited by user.
#  set(CONFIG_USE_middleware_edgefast_bluetooth_template true)

#  # description: edgefast_bluetooth porting
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting true)

#  # description: edgefast_bluetooth shell
#  set(CONFIG_USE_middleware_edgefast_bluetooth_shell true)

#  # description: edgefast_bluetooth ind_reset
#  set(CONFIG_USE_middleware_edgefast_bluetooth_controller_ind_reset true)

#  # description: edgefast_bluetooth shell
#  set(CONFIG_USE_middleware_edgefast_bluetooth_shell_le_audio true)

#  # description: edgefast_bluetooth liblc3codec
#  set(CONFIG_USE_middleware_edgefast_bluetooth_liblc3codec true)

#  # description: edgefast_bluetooth asrc_sw
#  set(CONFIG_USE_middleware_edgefast_bluetooth_asrc_sw true)

#  # description: edgefast_bluetooth shell_ble
#  set(CONFIG_USE_middleware_edgefast_bluetooth_shell_ble true)

#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind_config true)

#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_DEPEND_ANYOF_OPTION_1 true)

#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_DEPEND_ANYOF_OPTION_2 true)

#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_porting_DEPEND_ANYOF_OPTION_3 true)

#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_lwip_config true)

#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_wifi_config true)

#  # description: edgefast_bluetooth Abstraction Layer
#  set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_lc3_nodsp true)

#  # description: btble libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib true)

#  # description: BR libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib true)

#  # description: BLE libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib true)

#  # description: btble libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3 true)

#  # description: BR libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind_lib_lc3 true)

#  # description: BLE libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3 true)

#  # description: BLE libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_nodsp true)

#  # description: BLE libraries for bt_ble Host stack
#  set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_nodsp true)

#set.middleware.freertos-kernel
#  # description: FreeRTOS kernel
#  set(CONFIG_USE_middleware_freertos-kernel true)

#  # description: FreeRTOS cm33 non trustzone port
#  set(CONFIG_USE_middleware_freertos-kernel_cm33_non_trustzone true)

#  # description: FreeRTOS cm33 secure port
#  set(CONFIG_USE_middleware_freertos-kernel_cm33_trustzone_non_secure true)

#  # description: FreeRTOS Secure Context
#  set(CONFIG_USE_middleware_freertos-kernel_cm33_trustzone_secure true)

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

#set.middleware.corehttp
#  # description: Client implementation of the HTTP/1.1 specification for embedded devices.
#  set(CONFIG_USE_middleware_freertos_corehttp true)

#set.middleware.openvg
#  # description: Openvg library
#  set(CONFIG_USE_middleware_openvg true)

#set.middleware.audio_voice.components.vit
#  # description: Voice intelligent technology library
#  set(CONFIG_USE_middleware_audio_voice_components_vit_hifi4 true)

#  # description: Voice intelligent technology HiFi4 models
#  set(CONFIG_USE_middleware_audio_voice_components_vit_hifi4_models true)

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

#  # description: Middleware usb HSDCD (Select manually if needed)
#  set(CONFIG_USE_middleware_usb_hsdcd true)

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

#set.middleware.lwip
#  # description: lwIP - A Lightweight TCP/IP Stack
#  set(CONFIG_USE_middleware_lwip true)

#  # description: lwIP HTTP Daemon
#  set(CONFIG_USE_middleware_lwip_apps_httpd true)

#  # description: lwIP HTTP Daemon Implementation Support Files
#  set(CONFIG_USE_middleware_lwip_apps_httpd_support true)

#  # description: lwIP HTTP Server Implementation
#  set(CONFIG_USE_middleware_lwip_apps_httpsrv true)

#  # description: lwIP IPERF Implementation
#  set(CONFIG_USE_middleware_lwip_apps_lwiperf true)

#  # description: lwIP mDNS Implementation
#  set(CONFIG_USE_middleware_lwip_apps_mdns true)

#  # description: lwIP MQTT Client
#  set(CONFIG_USE_middleware_lwip_apps_mqtt true)

#  # description: lwIP SNTP
#  set(CONFIG_USE_middleware_lwip_apps_sntp true)

#  # description: lwIP Ping Sender contrib
#  set(CONFIG_USE_middleware_lwip_contrib_ping true)

#  # description: lwIP TCP Echo contrib
#  set(CONFIG_USE_middleware_lwip_contrib_tcpecho true)

#  # description: lwIP TCP Echo Raw API contrib
#  set(CONFIG_USE_middleware_lwip_contrib_tcpecho_raw true)

#  # description: lwIP UDP Echo contrib
#  set(CONFIG_USE_middleware_lwip_contrib_udpecho true)

#  # description: lwIP UDP Echo Raw API contrib
#  set(CONFIG_USE_middleware_lwip_contrib_udpecho_raw true)

#  # description: Empty ethernet interface
#  set(CONFIG_USE_middleware_lwip_empty_ethernetif true)

#  # description: EtherCAT EOE ethernet interface
#  set(CONFIG_USE_middleware_lwip_eoe_ethernetif true)

#  # description: lwIP System Abstraction Layer using either FreeRTOS dynamic memory allocation APIs or bare-metal
#  set(CONFIG_USE_middleware_lwip_sys_arch_dynamic true)

#  # description: Experimental lwIP System Abstraction Layer using FreeRTOS static memory allocation APIs - it is not supported and not guaranteed to work, also might be removed in future
#  set(CONFIG_USE_middleware_lwip_sys_arch_static true)

#  # description: lwIP Template
#  set(CONFIG_USE_middleware_lwip_template true)

#  # description: USB network layer
#  set(CONFIG_USE_middleware_lwip_usb_ethernetif true)

#  # description: Baremetal port layer for USB ethernet
#  set(CONFIG_USE_middleware_lwip_usb_ethernetif_bm true)

#  # description: FreeRTOS port layer for USB ethernet
#  set(CONFIG_USE_middleware_lwip_usb_ethernetif_freertos true)

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

#set.middleware.cadence.naturedsp_hifi4
#  # description: NatureDSP
#  set(CONFIG_USE_middleware_cadence_naturedsp_hifi4_rt700 true)

#set.middleware.cadence.naturedsp_hifi1
#  # description: NatureDSP
#  set(CONFIG_USE_middleware_cadence_naturedsp_hifi1 true)

#set.middleware.cadence.multicore_xaf
#  # description: DSP Audio Streamer
#  set(CONFIG_USE_middleware_cadence_multicore_xaf true)

#set.middleware.cadence.codecs_hifi4
#  # description: DSP Audio Streamer HiFi4 codecs
#  set(CONFIG_USE_middleware_cadence_codecs_hifi4 true)

#  # description: DSP Audio Streamer HiFi4 codecs for RT700
#  set(CONFIG_USE_middleware_cadence_codecs_hifi4_rt700 true)

#set.middleware.cadence.codecs_hifi1
#  # description: DSP Audio Streamer hifi1 codecs
#  set(CONFIG_USE_middleware_cadence_codecs_hifi1 true)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Core/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/DSP
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Driver
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/NN
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../arch/riscv/headers
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/mimxrt700evk/flash_config
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/mimxrt700evk/project_template
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/mimxrt700evk_a8974
  ${CMAKE_CURRENT_LIST_DIR}/../../components/audio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/aws_iot/kvstore
  ${CMAKE_CURRENT_LIST_DIR}/../../components/aws_iot/logging
  ${CMAKE_CURRENT_LIST_DIR}/../../components/aws_iot/mqtt-agent-interface
  ${CMAKE_CURRENT_LIST_DIR}/../../components/aws_iot/mqtt_agent
  ${CMAKE_CURRENT_LIST_DIR}/../../components/aws_iot/using_mbedtls_pkcs11
  ${CMAKE_CURRENT_LIST_DIR}/../../components/aws_iot/using_mbedtls_psa
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
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/ili9341
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/psp27801
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/ssd1963
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/st7796s
  ${CMAKE_CURRENT_LIST_DIR}/../../components/edgefast_wifi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/els_pkc
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/mflash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/gpio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/i2c
  ${CMAKE_CURRENT_LIST_DIR}/../../components/i3c_bus
  ${CMAKE_CURRENT_LIST_DIR}/../../components/led
  ${CMAKE_CURRENT_LIST_DIR}/../../components/lists
  ${CMAKE_CURRENT_LIST_DIR}/../../components/log
  ${CMAKE_CURRENT_LIST_DIR}/../../components/osa
  ${CMAKE_CURRENT_LIST_DIR}/../../components/panic
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pca9420
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pca9422
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf1550
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf3000
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf5020
  ${CMAKE_CURRENT_LIST_DIR}/../../components/power_manager/core
  ${CMAKE_CURRENT_LIST_DIR}/../../components/psa_crypto_driver
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
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/ov5640
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/ov7670
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/ov7725
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/device/sccb
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/camera/receiver
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/co5300
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dbi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dbi/flexio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dbi/lcdif
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc/dbi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc/dsi_cmd
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc/lcdif
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dc/ssd1963
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/fbdev
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/hx8394
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/mipi_dsi_cmd
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/rm67162
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/rm67191
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/rm68191
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/rm68200
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/rm692c9
  ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/rpi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/wifi_bt_module
  ${CMAKE_CURRENT_LIST_DIR}/../../components/wifi_bt_module/template
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/audio_voice/components/vit
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/device-defender
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/device-shadow
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/jobs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/ota
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/codecs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/multicore-xaf
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/naturedsp/hifi1
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/naturedsp/hifi4_rt700
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/nnlib
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/edgefast_bluetooth
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/edgefast_bluetooth/lib
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/eiq/mpp
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/eiq/tensorflow-lite
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/neutron
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/eiq/tensorflow-lite/third_party/neutron
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/emwin
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/fatfs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/freemaster
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/issdk
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/libjpeg
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/littlefs
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
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/lwip
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mbedtls3x
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/boot/bootutil
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/boot/nxp_mcux_sdk
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/boot/nxp_mcux_sdk/flashapi
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/ext/mbedtls-asn1
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/ext/nxp_encrypted_xip
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/ext/tinycrypt/lib
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/multicore
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/openvg
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/pkcs11
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/sdmmc
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tfm/psa-arch-tests/api-tests
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tfm/tf-m
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tinycbor
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/usb
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/vglite/driver
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/wifi_nxp
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/wireless/ethermind
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/backoffalgorithm
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/corehttp
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/corejson
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/coremqtt
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/coremqtt-agent
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/corepkcs11
  ${CMAKE_CURRENT_LIST_DIR}/../../rtos/freertos/freertos-kernel
  ${CMAKE_CURRENT_LIST_DIR}/cmsis_drivers
  ${CMAKE_CURRENT_LIST_DIR}/drivers
  ${CMAKE_CURRENT_LIST_DIR}/project_template
  ${CMAKE_CURRENT_LIST_DIR}/template
  ${CMAKE_CURRENT_LIST_DIR}/utilities
  ${CMAKE_CURRENT_LIST_DIR}/utilities/debug_console_lite
  ${CMAKE_CURRENT_LIST_DIR}/utilities/incbin
  ${CMAKE_CURRENT_LIST_DIR}/utilities/shell
  ${CMAKE_CURRENT_LIST_DIR}/utilities/unity
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
include_if_use(RISCV_Include_Core.MIMXRT798S)
include_if_use(board_project_template)
include_if_use(component_ak4497_adapter.MIMXRT798S)
include_if_use(component_audio_sai_edma_adapter.MIMXRT798S)
include_if_use(component_button.MIMXRT798S)
include_if_use(component_codec_adapters.MIMXRT798S)
include_if_use(component_codec_i2c.MIMXRT798S)
include_if_use(component_common_task)
include_if_use(component_cs42448_adapter.MIMXRT798S)
include_if_use(component_cs42888_adapter.MIMXRT798S)
include_if_use(component_ctimer_adapter.MIMXRT798S)
include_if_use(component_da7212_adapter.MIMXRT798S)
include_if_use(component_els_pkc)
include_if_use(component_els_pkc_aead)
include_if_use(component_els_pkc_aead_modes)
include_if_use(component_els_pkc_aes)
include_if_use(component_els_pkc_buffer)
include_if_use(component_els_pkc_cipher)
include_if_use(component_els_pkc_cipher_modes)
include_if_use(component_els_pkc_common)
include_if_use(component_els_pkc_core)
include_if_use(component_els_pkc_crc)
include_if_use(component_els_pkc_data_integrity)
include_if_use(component_els_pkc_doc_mimxrt)
include_if_use(component_els_pkc_ecc)
include_if_use(component_els_pkc_ecc_deterministic)
include_if_use(component_els_pkc_els)
include_if_use(component_els_pkc_els_common)
include_if_use(component_els_pkc_els_header_only)
include_if_use(component_els_pkc_examples)
include_if_use(component_els_pkc_examples_aead)
include_if_use(component_els_pkc_examples_cipher_modes)
include_if_use(component_els_pkc_examples_ecc)
include_if_use(component_els_pkc_examples_els)
include_if_use(component_els_pkc_examples_flow_protection)
include_if_use(component_els_pkc_examples_hash_modes)
include_if_use(component_els_pkc_examples_hmac)
include_if_use(component_els_pkc_examples_key)
include_if_use(component_els_pkc_examples_mac_modes)
include_if_use(component_els_pkc_examples_memory)
include_if_use(component_els_pkc_examples_random_modes)
include_if_use(component_els_pkc_examples_rsa)
include_if_use(component_els_pkc_flow_protection)
include_if_use(component_els_pkc_glikey)
include_if_use(component_els_pkc_hash)
include_if_use(component_els_pkc_hashmodes)
include_if_use(component_els_pkc_hmac)
include_if_use(component_els_pkc_key)
include_if_use(component_els_pkc_key_derivation)
include_if_use(component_els_pkc_mac)
include_if_use(component_els_pkc_mac_modes)
include_if_use(component_els_pkc_math)
include_if_use(component_els_pkc_memory)
include_if_use(component_els_pkc_oscca)
include_if_use(component_els_pkc_oscca_aeadmodes)
include_if_use(component_els_pkc_oscca_ciphermodes)
include_if_use(component_els_pkc_oscca_macmodes)
include_if_use(component_els_pkc_oscca_pkc)
include_if_use(component_els_pkc_oscca_randommodes)
include_if_use(component_els_pkc_oscca_safo)
include_if_use(component_els_pkc_oscca_sm2)
include_if_use(component_els_pkc_oscca_sm3)
include_if_use(component_els_pkc_oscca_sm4)
include_if_use(component_els_pkc_padding)
include_if_use(component_els_pkc_param_integrity)
include_if_use(component_els_pkc_pkc)
include_if_use(component_els_pkc_platform_mimxrt)
include_if_use(component_els_pkc_pre_processor)
include_if_use(component_els_pkc_prng)
include_if_use(component_els_pkc_psa_driver)
include_if_use(component_els_pkc_random)
include_if_use(component_els_pkc_random_modes)
include_if_use(component_els_pkc_random_modes_ctr)
include_if_use(component_els_pkc_random_modes_hmacdrbg)
include_if_use(component_els_pkc_rsa)
include_if_use(component_els_pkc_rsa_oaep)
include_if_use(component_els_pkc_secure_counter)
include_if_use(component_els_pkc_session)
include_if_use(component_els_pkc_signature)
include_if_use(component_els_pkc_standalone_gdet)
include_if_use(component_els_pkc_static_lib_mimxrt)
include_if_use(component_els_pkc_toolchain)
include_if_use(component_els_pkc_trng)
include_if_use(component_els_pkc_trng_type_els)
include_if_use(component_els_pkc_trng_type_rng4)
include_if_use(component_flash_nor_xspi.MIMXRT798S)
include_if_use(component_gpio_adapter.MIMXRT798S)
include_if_use(component_i2c_adapter_interface.MIMXRT798S)
include_if_use(component_i3c_adapter.MIMXRT798S)
include_if_use(component_i3c_bus.MIMXRT798S)
include_if_use(component_i3c_bus_adapter.MIMXRT798S)
include_if_use(component_led.MIMXRT798S)
include_if_use(component_lists.MIMXRT798S)
include_if_use(component_log.MIMXRT798S)
include_if_use(component_log_backend_debug_console.MIMXRT798S)
include_if_use(component_log_backend_debug_console_lite.MIMXRT798S)
include_if_use(component_log_backend_ringbuffer.MIMXRT798S)
include_if_use(component_lpi2c_adapter.MIMXRT798S)
include_if_use(component_lpuart_adapter.MIMXRT798S)
include_if_use(component_mflash_offchip.MIMXRT798S)
include_if_use(component_mflash_onchip.MIMXRT798S)
include_if_use(component_mrt_adapter.MIMXRT798S)
include_if_use(component_osa)
include_if_use(component_osa_bm)
include_if_use(component_osa_free_rtos)
include_if_use(component_osa_interface)
include_if_use(component_osa_template_config)
include_if_use(component_ostimer_adapter.MIMXRT798S)
include_if_use(component_panic.MIMXRT798S)
include_if_use(component_pcm186x_adapter.MIMXRT798S)
include_if_use(component_pcm512x_adapter.MIMXRT798S)
include_if_use(component_power_manager_framework.MIMXRT798S)
include_if_use(component_psa_crypto_driver_ele_s200_common)
include_if_use(component_psa_crypto_driver_ele_s200_transparent)
include_if_use(component_psa_crypto_driver_els_pkc)
include_if_use(component_psa_crypto_driver_els_pkc_common)
include_if_use(component_psa_crypto_driver_els_pkc_opaque)
include_if_use(component_psa_crypto_driver_els_pkc_oracle)
include_if_use(component_psa_crypto_driver_els_pkc_oracle_mcxn)
include_if_use(component_psa_crypto_driver_els_pkc_oracle_mimxrt)
include_if_use(component_psa_crypto_driver_els_pkc_oracle_rw61x)
include_if_use(component_psa_crypto_driver_els_pkc_transparent)
include_if_use(component_psa_crypto_driver_mbedtls_thread_alt)
include_if_use(component_psa_crypto_driver_osal)
include_if_use(component_psa_crypto_driver_osal_baremetal)
include_if_use(component_psa_crypto_driver_osal_frtos)
include_if_use(component_psa_crypto_driver_use_trng_compile_option)
include_if_use(component_reset_adapter.MIMXRT798S)
include_if_use(component_serial_manager.MIMXRT798S)
include_if_use(component_serial_manager_swo.MIMXRT798S)
include_if_use(component_serial_manager_uart.MIMXRT798S)
include_if_use(component_serial_manager_usb_cdc.MIMXRT798S)
include_if_use(component_serial_manager_virtual.MIMXRT798S)
include_if_use(component_sgtl_adapter.MIMXRT798S)
include_if_use(component_software_rng_adapter.MIMXRT798S)
include_if_use(component_tfa9896_adapter.MIMXRT798S)
include_if_use(component_tfa9xxx_adapter.MIMXRT798S)
include_if_use(component_timer_manager.MIMXRT798S)
include_if_use(component_trng_adapter.MIMXRT798S)
include_if_use(component_wifi_bt_module_config.MIMXRT798S)
include_if_use(component_wifi_bt_module_tx_pwr_limits.MIMXRT798S)
include_if_use(component_wm8524_adapter.MIMXRT798S)
include_if_use(component_wm8904_adapter.MIMXRT798S)
include_if_use(component_wm8960_adapter.MIMXRT798S)
include_if_use(component_wm8962_adapter.MIMXRT798S)
include_if_use(device_CMSIS.MIMXRT798S)
include_if_use(device_RTE.MIMXRT798S)
include_if_use(device_project_template.MIMXRT798S)
include_if_use(device_startup.MIMXRT798S)
include_if_use(device_system.MIMXRT798S)
include_if_use(driver_acmp.MIMXRT798S)
include_if_use(driver_ak4497.MIMXRT798S)
include_if_use(driver_cache_xcache.MIMXRT798S)
include_if_use(driver_camera-common.MIMXRT798S)
include_if_use(driver_camera-device-common.MIMXRT798S)
include_if_use(driver_camera-device-ov5640.MIMXRT798S)
include_if_use(driver_camera-device-ov7670.MIMXRT798S)
include_if_use(driver_camera-device-ov7725.MIMXRT798S)
include_if_use(driver_camera-device-sccb.MIMXRT798S)
include_if_use(driver_camera-receiver-common.MIMXRT798S)
include_if_use(driver_cdog.MIMXRT798S)
include_if_use(driver_clock.MIMXRT798S)
include_if_use(driver_cmsis_lpi2c.MIMXRT798S)
include_if_use(driver_cmsis_lpspi.MIMXRT798S)
include_if_use(driver_cmsis_lpuart.MIMXRT798S)
include_if_use(driver_codec.MIMXRT798S)
include_if_use(driver_common.MIMXRT798S)
include_if_use(driver_crc.MIMXRT798S)
include_if_use(driver_cs42448.MIMXRT798S)
include_if_use(driver_cs42888.MIMXRT798S)
include_if_use(driver_ctimer.MIMXRT798S)
include_if_use(driver_dbi.MIMXRT798S)
include_if_use(driver_dbi_flexio_edma.MIMXRT798S)
include_if_use(driver_dbi_lcdif.MIMXRT798S)
include_if_use(driver_dc-fb-common.MIMXRT798S)
include_if_use(driver_dc-fb-dbi.MIMXRT798S)
include_if_use(driver_dc-fb-dsi-cmd.MIMXRT798S)
include_if_use(driver_dc-fb-lcdif.MIMXRT798S)
include_if_use(driver_dc-fb-ssd1963.MIMXRT798S)
include_if_use(driver_dialog7212.MIMXRT798S)
include_if_use(driver_display-co5300.MIMXRT798S)
include_if_use(driver_display-common.MIMXRT798S)
include_if_use(driver_display-hx8394.MIMXRT798S)
include_if_use(driver_display-mipi-dsi-cmd.MIMXRT798S)
include_if_use(driver_display-rm67162.MIMXRT798S)
include_if_use(driver_display-rm67191.MIMXRT798S)
include_if_use(driver_display-rm68191.MIMXRT798S)
include_if_use(driver_display-rm68200.MIMXRT798S)
include_if_use(driver_display-rm692c9.MIMXRT798S)
include_if_use(driver_display-rpi.MIMXRT798S)
include_if_use(driver_dsp.MIMXRT798S)
include_if_use(driver_edma4.MIMXRT798S)
include_if_use(driver_edma_soc.MIMXRT798S)
include_if_use(driver_ektf2k.MIMXRT798S)
include_if_use(driver_ezhv.MIMXRT798S)
include_if_use(driver_fbdev.MIMXRT798S)
include_if_use(driver_flash_config)
include_if_use(driver_flexio.MIMXRT798S)
include_if_use(driver_flexio_camera.MIMXRT798S)
include_if_use(driver_flexio_i2c_master.MIMXRT798S)
include_if_use(driver_flexio_i2s.MIMXRT798S)
include_if_use(driver_flexio_i2s_edma.MIMXRT798S)
include_if_use(driver_flexio_mculcd.MIMXRT798S)
include_if_use(driver_flexio_mculcd_edma.MIMXRT798S)
include_if_use(driver_flexio_spi.MIMXRT798S)
include_if_use(driver_flexio_spi_edma.MIMXRT798S)
include_if_use(driver_flexio_uart.MIMXRT798S)
include_if_use(driver_flexio_uart_edma.MIMXRT798S)
include_if_use(driver_ft3267.MIMXRT798S)
include_if_use(driver_ft5406.MIMXRT798S)
include_if_use(driver_ft5406_rt.MIMXRT798S)
include_if_use(driver_ft6x06.MIMXRT798S)
include_if_use(driver_fxas21002cq.MIMXRT798S)
include_if_use(driver_fxls8974cf.MIMXRT798S)
include_if_use(driver_fxos8700cq.MIMXRT798S)
include_if_use(driver_gdet.MIMXRT798S)
include_if_use(driver_glikey.MIMXRT798S)
include_if_use(driver_gpio.MIMXRT798S)
include_if_use(driver_gt911.MIMXRT798S)
include_if_use(driver_htu21d.MIMXRT798S)
include_if_use(driver_i3c.MIMXRT798S)
include_if_use(driver_i3c_edma.MIMXRT798S)
include_if_use(driver_icm42688p.MIMXRT798S)
include_if_use(driver_ili9341.MIMXRT798S)
include_if_use(driver_inputmux.MIMXRT798S)
include_if_use(driver_inputmux_connections.MIMXRT798S)
include_if_use(driver_iopctl_soc.MIMXRT798S)
include_if_use(driver_irtc.MIMXRT798S)
include_if_use(driver_itrc.MIMXRT798S)
include_if_use(driver_jpegdec.MIMXRT798S)
include_if_use(driver_lcdif.MIMXRT798S)
include_if_use(driver_lpadc.MIMXRT798S)
include_if_use(driver_lpc_freqme.MIMXRT798S)
include_if_use(driver_lpflexcomm.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpi2c.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpi2c_edma.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpi2c_freertos.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpspi.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpspi_edma.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpspi_freertos.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpuart.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpuart_edma.MIMXRT798S)
include_if_use(driver_lpflexcomm_lpuart_freertos.MIMXRT798S)
include_if_use(driver_lsm6dso.MIMXRT798S)
include_if_use(driver_max30101.MIMXRT798S)
include_if_use(driver_mipi_dsi.MIMXRT798S)
include_if_use(driver_mma8451q.MIMXRT798S)
include_if_use(driver_mma8652fc.MIMXRT798S)
include_if_use(driver_mmu.MIMXRT798S)
include_if_use(driver_mrt.MIMXRT798S)
include_if_use(driver_mu1.MIMXRT798S)
include_if_use(driver_ostimer.MIMXRT798S)
include_if_use(driver_p3t1755.MIMXRT798S)
include_if_use(driver_pca9420.MIMXRT798S)
include_if_use(driver_pca9422.MIMXRT798S)
include_if_use(driver_pcm186x.MIMXRT798S)
include_if_use(driver_pcm512x.MIMXRT798S)
include_if_use(driver_pdm.MIMXRT798S)
include_if_use(driver_pdm_edma.MIMXRT798S)
include_if_use(driver_pf1550.MIMXRT798S)
include_if_use(driver_pf3000.MIMXRT798S)
include_if_use(driver_pf5020.MIMXRT798S)
include_if_use(driver_pint.MIMXRT798S)
include_if_use(driver_pngdec.MIMXRT798S)
include_if_use(driver_power.MIMXRT798S)
include_if_use(driver_psp27801.MIMXRT798S)
include_if_use(driver_puf_v3.MIMXRT798S)
include_if_use(driver_reset.MIMXRT798S)
include_if_use(driver_romapi.MIMXRT798S)
include_if_use(driver_rtt.MIMXRT798S)
include_if_use(driver_rtt_template.MIMXRT798S)
include_if_use(driver_sai.MIMXRT798S)
include_if_use(driver_sai_edma.MIMXRT798S)
include_if_use(driver_sctimer.MIMXRT798S)
include_if_use(driver_sdadc.MIMXRT798S)
include_if_use(driver_sema42.MIMXRT798S)
include_if_use(driver_sgtl5000.MIMXRT798S)
include_if_use(driver_soc_mipi_dsi.MIMXRT798S)
include_if_use(driver_ssd1963.MIMXRT798S)
include_if_use(driver_st7796s.MIMXRT798S)
include_if_use(driver_syspm.MIMXRT798S)
include_if_use(driver_tfa9896.MIMXRT798S)
include_if_use(driver_tfa9xxx.MIMXRT798S)
include_if_use(driver_tfa9xxx_hal.MIMXRT798S)
include_if_use(driver_tma525b.MIMXRT798S)
include_if_use(driver_trng.MIMXRT798S)
include_if_use(driver_tsl2561.MIMXRT798S)
include_if_use(driver_usdhc.MIMXRT798S)
include_if_use(driver_utick.MIMXRT798S)
include_if_use(driver_video-common.MIMXRT798S)
include_if_use(driver_wm8524.MIMXRT798S)
include_if_use(driver_wm8904.MIMXRT798S)
include_if_use(driver_wm8960.MIMXRT798S)
include_if_use(driver_wm8962.MIMXRT798S)
include_if_use(driver_wwdt.MIMXRT798S)
include_if_use(driver_xspi.MIMXRT798S)
include_if_use(driver_xspi_edma.MIMXRT798S)
include_if_use(kit_project_template_mimxrt700evk_a8974)
include_if_use(middleware_audio_voice_components_vit_hifi4)
include_if_use(middleware_audio_voice_components_vit_hifi4_models)
include_if_use(middleware_aws_iot_device_defender)
include_if_use(middleware_aws_iot_device_shadow)
include_if_use(middleware_aws_iot_device_shadow_template)
include_if_use(middleware_aws_iot_jobs)
include_if_use(middleware_aws_iot_ota)
include_if_use(middleware_aws_iot_ota_freertos)
include_if_use(middleware_cadence_codecs_hifi1)
include_if_use(middleware_cadence_codecs_hifi4)
include_if_use(middleware_cadence_codecs_hifi4_rt700)
include_if_use(middleware_cadence_multicore_xaf)
include_if_use(middleware_cadence_naturedsp_hifi1)
include_if_use(middleware_cadence_naturedsp_hifi4_rt700)
include_if_use(middleware_cadence_nnlib)
include_if_use(middleware_cadence_nnlib_rt700_hifi1)
include_if_use(middleware_cadence_nnlib_rt700_hifi4)
include_if_use(middleware_edgefast_bluetooth_asrc_sw)
include_if_use(middleware_edgefast_bluetooth_ble_ethermind)
include_if_use(middleware_edgefast_bluetooth_ble_ethermind_lib)
include_if_use(middleware_edgefast_bluetooth_ble_ethermind_lib_lc3)
include_if_use(middleware_edgefast_bluetooth_ble_ethermind_lib_lc3_nodsp)
include_if_use(middleware_edgefast_bluetooth_br_ethermind)
include_if_use(middleware_edgefast_bluetooth_br_ethermind_lib)
include_if_use(middleware_edgefast_bluetooth_br_ethermind_lib_lc3)
include_if_use(middleware_edgefast_bluetooth_btble_ethermind)
include_if_use(middleware_edgefast_bluetooth_btble_ethermind_lib)
include_if_use(middleware_edgefast_bluetooth_btble_ethermind_lib_lc3)
include_if_use(middleware_edgefast_bluetooth_btble_ethermind_lib_lc3_nodsp)
include_if_use(middleware_edgefast_bluetooth_common_ethermind)
include_if_use(middleware_edgefast_bluetooth_common_ethermind_hci)
include_if_use(middleware_edgefast_bluetooth_common_ethermind_hci_uart)
include_if_use(middleware_edgefast_bluetooth_common_ethermind_lc3)
include_if_use(middleware_edgefast_bluetooth_common_ethermind_lc3_nodsp)
include_if_use(middleware_edgefast_bluetooth_config_ethermind)
include_if_use(middleware_edgefast_bluetooth_controller_ind_reset)
include_if_use(middleware_edgefast_bluetooth_extension_common_ethermind)
include_if_use(middleware_edgefast_bluetooth_k32w061_controller)
include_if_use(middleware_edgefast_bluetooth_le_audio_aics)
include_if_use(middleware_edgefast_bluetooth_le_audio_bap)
include_if_use(middleware_edgefast_bluetooth_le_audio_bass)
include_if_use(middleware_edgefast_bluetooth_le_audio_cap)
include_if_use(middleware_edgefast_bluetooth_le_audio_ccid)
include_if_use(middleware_edgefast_bluetooth_le_audio_config_template)
include_if_use(middleware_edgefast_bluetooth_le_audio_csip)
include_if_use(middleware_edgefast_bluetooth_le_audio_gmap)
include_if_use(middleware_edgefast_bluetooth_le_audio_has)
include_if_use(middleware_edgefast_bluetooth_le_audio_mcc)
include_if_use(middleware_edgefast_bluetooth_le_audio_mcs)
include_if_use(middleware_edgefast_bluetooth_le_audio_micp)
include_if_use(middleware_edgefast_bluetooth_le_audio_mpl)
include_if_use(middleware_edgefast_bluetooth_le_audio_pbp)
include_if_use(middleware_edgefast_bluetooth_le_audio_tbs)
include_if_use(middleware_edgefast_bluetooth_le_audio_tmap)
include_if_use(middleware_edgefast_bluetooth_le_audio_vcp)
include_if_use(middleware_edgefast_bluetooth_le_audio_vocs)
include_if_use(middleware_edgefast_bluetooth_liblc3codec)
include_if_use(middleware_edgefast_bluetooth_pal)
include_if_use(middleware_edgefast_bluetooth_pal_crypto_ethermind)
include_if_use(middleware_edgefast_bluetooth_pal_db_gen_ethermind)
include_if_use(middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind)
include_if_use(middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind_config)
include_if_use(middleware_edgefast_bluetooth_pal_platform_ethermind)
include_if_use(middleware_edgefast_bluetooth_porting)
include_if_use(middleware_edgefast_bluetooth_porting_DEPEND_ANYOF_OPTION_1)
include_if_use(middleware_edgefast_bluetooth_porting_DEPEND_ANYOF_OPTION_2)
include_if_use(middleware_edgefast_bluetooth_porting_DEPEND_ANYOF_OPTION_3)
include_if_use(middleware_edgefast_bluetooth_porting_atomic)
include_if_use(middleware_edgefast_bluetooth_porting_list)
include_if_use(middleware_edgefast_bluetooth_porting_net)
include_if_use(middleware_edgefast_bluetooth_porting_toolchain)
include_if_use(middleware_edgefast_bluetooth_porting_work_queue)
include_if_use(middleware_edgefast_bluetooth_profile_bas)
include_if_use(middleware_edgefast_bluetooth_profile_dis)
include_if_use(middleware_edgefast_bluetooth_profile_fmp)
include_if_use(middleware_edgefast_bluetooth_profile_hps)
include_if_use(middleware_edgefast_bluetooth_profile_hrs)
include_if_use(middleware_edgefast_bluetooth_profile_hts)
include_if_use(middleware_edgefast_bluetooth_profile_ias)
include_if_use(middleware_edgefast_bluetooth_profile_ipsp)
include_if_use(middleware_edgefast_bluetooth_profile_ots)
include_if_use(middleware_edgefast_bluetooth_profile_pxr)
include_if_use(middleware_edgefast_bluetooth_profile_tip)
include_if_use(middleware_edgefast_bluetooth_profile_wu)
include_if_use(middleware_edgefast_bluetooth_rw610_controller)
include_if_use(middleware_edgefast_bluetooth_shell)
include_if_use(middleware_edgefast_bluetooth_shell_ble)
include_if_use(middleware_edgefast_bluetooth_shell_le_audio)
include_if_use(middleware_edgefast_bluetooth_template)
include_if_use(middleware_edgefast_bluetooth_wifi_nxp_controller_base)
include_if_use(middleware_edgefast_bluetooth_wifi_nxp_controller_lwip_config)
include_if_use(middleware_edgefast_bluetooth_wifi_nxp_controller_wifi_config)
include_if_use(middleware_edgefast_wifi_nxp)
include_if_use(middleware_eiq_mpp)
include_if_use(middleware_eiq_tensorflow_lite_micro)
include_if_use(middleware_eiq_tensorflow_lite_micro_binary)
include_if_use(middleware_eiq_tensorflow_lite_micro_cmsis_nn)
include_if_use(middleware_eiq_tensorflow_lite_micro_debug_log)
include_if_use(middleware_eiq_tensorflow_lite_micro_examples_microspeech)
include_if_use(middleware_eiq_tensorflow_lite_micro_headers)
include_if_use(middleware_eiq_tensorflow_lite_micro_library)
include_if_use(middleware_eiq_tensorflow_lite_micro_neutron)
include_if_use(middleware_eiq_tensorflow_lite_micro_refenerce)
include_if_use(middleware_eiq_tensorflow_lite_micro_reference)
include_if_use(middleware_eiq_tensorflow_lite_micro_source)
include_if_use(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn)
include_if_use(middleware_eiq_tensorflow_lite_micro_third_party_neutron_lib_binary)
include_if_use(middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4)
include_if_use(middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4_binary)
include_if_use(middleware_eiq_tensorflow_lite_micro_xtensa)
include_if_use(middleware_emwin)
include_if_use(middleware_emwin_template)
include_if_use(middleware_fatfs)
include_if_use(middleware_fatfs_mmc)
include_if_use(middleware_fatfs_ram)
include_if_use(middleware_fatfs_sd)
include_if_use(middleware_fatfs_sdspi)
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
include_if_use(middleware_freertos-kernel_cm33_non_trustzone)
include_if_use(middleware_freertos-kernel_cm33_trustzone_non_secure)
include_if_use(middleware_freertos-kernel_cm33_trustzone_secure)
include_if_use(middleware_freertos-kernel_config)
include_if_use(middleware_freertos-kernel_extension)
include_if_use(middleware_freertos-kernel_heap_1)
include_if_use(middleware_freertos-kernel_heap_2)
include_if_use(middleware_freertos-kernel_heap_3)
include_if_use(middleware_freertos-kernel_heap_4)
include_if_use(middleware_freertos-kernel_heap_5)
include_if_use(middleware_freertos-kernel_mpu_wrappers)
include_if_use(middleware_freertos-kernel_mpu_wrappers_v2)
include_if_use(middleware_freertos_backoffalgorithm)
include_if_use(middleware_freertos_corehttp)
include_if_use(middleware_freertos_corejson)
include_if_use(middleware_freertos_coremqtt)
include_if_use(middleware_freertos_coremqtt-agent)
include_if_use(middleware_freertos_coremqtt_template)
include_if_use(middleware_freertos_corepkcs11)
include_if_use(middleware_freertos_corepkcs11_mbedtls)
include_if_use(middleware_freertos_corepkcs11_mbedtls_utils)
include_if_use(middleware_freertos_corepkcs11_template)
include_if_use(middleware_iot_reference_kvstore)
include_if_use(middleware_iot_reference_logging)
include_if_use(middleware_iot_reference_mqtt_agent)
include_if_use(middleware_iot_reference_mqtt_agent_interface)
include_if_use(middleware_iot_reference_transport_mbedtls3x)
include_if_use(middleware_iot_reference_transport_mbedtls_psa)
include_if_use(middleware_issdk_drivers_gpio_imxrt700)
include_if_use(middleware_issdk_drivers_systick_utils)
include_if_use(middleware_issdk_sensor_allregdefs)
include_if_use(middleware_issdk_sensor_fxls8974cf)
include_if_use(middleware_issdk_sensor_interface_common)
include_if_use(middleware_libjpeg)
include_if_use(middleware_libjpeg_template)
include_if_use(middleware_littlefs)
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
include_if_use(middleware_lwip)
include_if_use(middleware_lwip_apps_httpd)
include_if_use(middleware_lwip_apps_httpd_support)
include_if_use(middleware_lwip_apps_httpsrv)
include_if_use(middleware_lwip_apps_lwiperf)
include_if_use(middleware_lwip_apps_mdns)
include_if_use(middleware_lwip_apps_mqtt)
include_if_use(middleware_lwip_apps_sntp)
include_if_use(middleware_lwip_contrib_ping)
include_if_use(middleware_lwip_contrib_tcpecho)
include_if_use(middleware_lwip_contrib_tcpecho_raw)
include_if_use(middleware_lwip_contrib_udpecho)
include_if_use(middleware_lwip_contrib_udpecho_raw)
include_if_use(middleware_lwip_empty_ethernetif)
include_if_use(middleware_lwip_eoe_ethernetif)
include_if_use(middleware_lwip_sys_arch_dynamic)
include_if_use(middleware_lwip_sys_arch_static)
include_if_use(middleware_lwip_template)
include_if_use(middleware_lwip_usb_ethernetif)
include_if_use(middleware_lwip_usb_ethernetif_bm)
include_if_use(middleware_lwip_usb_ethernetif_freertos)
include_if_use(middleware_mbedtls3x)
include_if_use(middleware_mbedtls3x_crypto)
include_if_use(middleware_mbedtls3x_crypto_no_psa)
include_if_use(middleware_mbedtls3x_crypto_storage_default)
include_if_use(middleware_mbedtls3x_crypto_storage_ram)
include_if_use(middleware_mbedtls3x_no_psa)
include_if_use(middleware_mbedtls3x_port_config)
include_if_use(middleware_mbedtls3x_port_hw_init)
include_if_use(middleware_mbedtls3x_port_psa_crypto_config)
include_if_use(middleware_mbedtls3x_ssl)
include_if_use(middleware_mbedtls3x_ssl_no_psa)
include_if_use(middleware_mbedtls3x_template)
include_if_use(middleware_mbedtls3x_tests)
include_if_use(middleware_mbedtls3x_x509)
include_if_use(middleware_mcuboot_bootutil)
include_if_use(middleware_mcuboot_bootutil_flashapi_mcx)
include_if_use(middleware_mcuboot_bootutil_flashapi_rt)
include_if_use(middleware_mcuboot_mbedtls-asn1)
include_if_use(middleware_mcuboot_nxp_app_support)
include_if_use(middleware_mcuboot_nxp_bootutil_port)
include_if_use(middleware_mcuboot_nxp_encrypted_xip)
include_if_use(middleware_mcuboot_stack_heap_default_mbedtls)
include_if_use(middleware_mcuboot_stack_heap_default_tinycrypt)
include_if_use(middleware_mcuboot_tinycrypt)
include_if_use(middleware_multicore_erpc)
include_if_use(middleware_multicore_erpc_arbitrator)
include_if_use(middleware_multicore_erpc_client)
include_if_use(middleware_multicore_erpc_doc)
include_if_use(middleware_multicore_erpc_eRPC_dspi_master_transport)
include_if_use(middleware_multicore_erpc_eRPC_dspi_slave_transport)
include_if_use(middleware_multicore_erpc_eRPC_i2c_slave_transport)
include_if_use(middleware_multicore_erpc_eRPC_lpi2c_slave_transport)
include_if_use(middleware_multicore_erpc_eRPC_lpspi_slave_transport)
include_if_use(middleware_multicore_erpc_eRPC_mu_transport)
include_if_use(middleware_multicore_erpc_eRPC_rpmsg_lite_rtos_transport_master)
include_if_use(middleware_multicore_erpc_eRPC_rpmsg_lite_rtos_transport_remote)
include_if_use(middleware_multicore_erpc_eRPC_rpmsg_lite_transport_master)
include_if_use(middleware_multicore_erpc_eRPC_rpmsg_lite_transport_remote)
include_if_use(middleware_multicore_erpc_eRPC_rpmsg_tty_rtos_transport)
include_if_use(middleware_multicore_erpc_eRPC_spi_master_transport)
include_if_use(middleware_multicore_erpc_eRPC_spi_slave_transport)
include_if_use(middleware_multicore_erpc_eRPC_uart_cmsis_transport)
include_if_use(middleware_multicore_erpc_examples_erpc_common_multicore)
include_if_use(middleware_multicore_erpc_examples_erpc_matrix_multiply_client)
include_if_use(middleware_multicore_erpc_examples_erpc_matrix_multiply_server)
include_if_use(middleware_multicore_erpc_examples_erpc_two_way_rpc_core0)
include_if_use(middleware_multicore_erpc_examples_erpc_two_way_rpc_core1)
include_if_use(middleware_multicore_erpc_port_freertos)
include_if_use(middleware_multicore_erpc_port_stdlib)
include_if_use(middleware_multicore_erpc_server)
include_if_use(middleware_multicore_mcmgr)
include_if_use(middleware_multicore_mcmgr_imxrt700)
include_if_use(middleware_multicore_rpmsg-lite)
include_if_use(middleware_multicore_rpmsg-lite_bm)
include_if_use(middleware_multicore_rpmsg-lite_freertos)
include_if_use(middleware_multicore_rpmsg-lite_imxrt700)
include_if_use(middleware_multicore_rpmsg-lite_imxrt700_hifi1)
include_if_use(middleware_multicore_rpmsg-lite_imxrt700_hifi4)
include_if_use(middleware_multicore_rpmsg-lite_ns)
include_if_use(middleware_multicore_rpmsg-lite_qnx)
include_if_use(middleware_multicore_rpmsg-lite_queue)
include_if_use(middleware_multicore_rpmsg-lite_threadx)
include_if_use(middleware_multicore_rpmsg-lite_xos)
include_if_use(middleware_multicore_rpmsg-lite_zephyr)
include_if_use(middleware_openvg)
include_if_use(middleware_pkcs11)
include_if_use(middleware_psatest_ns)
include_if_use(middleware_psatest_ns_crypto)
include_if_use(middleware_psatest_ns_internal_trusted_storage)
include_if_use(middleware_psatest_ns_protected_storage)
include_if_use(middleware_psatest_ns_t_cose)
include_if_use(middleware_psatest_ns_tgt_dev_apis_nxp)
include_if_use(middleware_sdmmc_common)
include_if_use(middleware_sdmmc_host_usdhc)
include_if_use(middleware_sdmmc_host_usdhc_blocking)
include_if_use(middleware_sdmmc_host_usdhc_non_blocking)
include_if_use(middleware_sdmmc_mmc)
include_if_use(middleware_sdmmc_osa)
include_if_use(middleware_sdmmc_sd)
include_if_use(middleware_sdmmc_sdhc_template)
include_if_use(middleware_sdmmc_sdif_template)
include_if_use(middleware_sdmmc_sdio)
include_if_use(middleware_sdmmc_sdspi)
include_if_use(middleware_sdmmc_usdhc_template)
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
include_if_use(middleware_usb_hsdcd)
include_if_use(middleware_usb_pd)
include_if_use(middleware_usb_pd_altmode)
include_if_use(middleware_usb_pd_compliance_test_report)
include_if_use(middleware_usb_pd_phy_ptn5110)
include_if_use(middleware_usb_phy)
include_if_use(middleware_vglite)
include_if_use(middleware_wifi)
include_if_use(middleware_wifi_cli)
include_if_use(middleware_wifi_common_files)
include_if_use(middleware_wifi_firmware_download)
include_if_use(middleware_wifi_fwdnld)
include_if_use(middleware_wifi_fwdnld_intf_abs)
include_if_use(middleware_wifi_mlan_sdio)
include_if_use(middleware_wifi_ncp_supp_wmcrypto)
include_if_use(middleware_wifi_net)
include_if_use(middleware_wifi_net_free_rtos)
include_if_use(middleware_wifi_net_thread)
include_if_use(middleware_wifi_osa)
include_if_use(middleware_wifi_osa_free_rtos)
include_if_use(middleware_wifi_osa_thread)
include_if_use(middleware_wifi_sdio)
include_if_use(middleware_wifi_template)
include_if_use(middleware_wifi_wifi_bt_config_template)
include_if_use(middleware_wifi_wifidriver)
include_if_use(middleware_wifi_wifidriver_softap)
include_if_use(middleware_wifi_wls)
include_if_use(middleware_wifi_wmcrypto)
include_if_use(middleware_wifi_wmtime)
include_if_use(utilities_misc_utilities.MIMXRT798S)
include_if_use(utilities_unity.MIMXRT798S)
include_if_use(utility_assert.MIMXRT798S)
include_if_use(utility_assert_lite.MIMXRT798S)
include_if_use(utility_debug_console.MIMXRT798S)
include_if_use(utility_debug_console_lite.MIMXRT798S)
include_if_use(utility_debug_console_template_config.MIMXRT798S)
include_if_use(utility_incbin.MIMXRT798S)
include_if_use(utility_shell.MIMXRT798S)
include_if_use(utility_str.MIMXRT798S)
