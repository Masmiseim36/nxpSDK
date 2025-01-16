# Copy variable into project config.cmake to use software component
#set.board.evkmimxrt685
#  # description: flash config block
#  set(CONFIG_USE_driver_flash_config true)

#  # description: Board_project_template evkmimxrt685
#  set(CONFIG_USE_board_project_template true)

#set.board.mimxrt685audevk
#  # description: flash config block
#  set(CONFIG_USE_driver_flash_config true)

#  # description: Board_project_template mimxrt685audevk
#  set(CONFIG_USE_board_project_template true)

#set.device.MIMXRT685S
#  # description: Utilities which is needed for particular toolchain like the SBRK function required to address limitation between HEAP and STACK in GCC toolchain library.
#  set(CONFIG_USE_utilities_misc_utilities true)

#  # description: Used to include slave core binary into master core binary.
#  set(CONFIG_USE_utility_incbin true)

#  # description: Utility debug_console Template Config
#  set(CONFIG_USE_utility_debug_console_template_config true)

#  # description: Clock Driver
#  set(CONFIG_USE_driver_clock true)

#  # description: I2S Bridge Driver
#  set(CONFIG_USE_driver_i2s_bridge true)

#  # description: Inputmux_connections Driver
#  set(CONFIG_USE_driver_inputmux_connections true)

#  # description: Reset Driver
#  set(CONFIG_USE_driver_reset true)

#  # description: common Driver
#  set(CONFIG_USE_driver_common true)

#  # description: Wi-Fi and BT module configs
#  set(CONFIG_USE_component_wifi_bt_module_config true)

#  # description: Wi-Fi module Tx power limits
#  set(CONFIG_USE_component_wifi_bt_module_tx_pwr_limits true)

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

#  # description: Component software_rng_adapter
#  set(CONFIG_USE_component_software_rng_adapter true)

#  # description: Component reset_adapter
#  set(CONFIG_USE_component_reset_adapter true)

#  # description: Component panic
#  set(CONFIG_USE_component_panic true)

#  # description: Component mpi_loader
#  set(CONFIG_USE_component_mpi_loader true)

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

#  # description: Rte_device
#  set(CONFIG_USE_device_RTE true)

#  # description: Devices_project_template MIMXRT685S
#  set(CONFIG_USE_device_project_template true)

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

#  # description: Device MIMXRT685S_startup
#  set(CONFIG_USE_device_startup true)

#  # description: dsp Driver
#  set(CONFIG_USE_driver_dsp true)

#  # description: Power driver
#  set(CONFIG_USE_driver_power true)

#  # description: FLEXCOMM USART FreeRTOS Driver
#  set(CONFIG_USE_driver_flexcomm_usart_freertos true)

#  # description: FLEXCOMM SPI FreeRTOS Driver
#  set(CONFIG_USE_driver_flexcomm_spi_freertos true)

#  # description: FLEXCOMM I2C FreeRTOS Driver
#  set(CONFIG_USE_driver_flexcomm_i2c_freertos true)

#  # description: WWDT Driver
#  set(CONFIG_USE_driver_wwdt true)

#  # description: UTICK Driver
#  set(CONFIG_USE_driver_utick true)

#  # description: USDHC Driver
#  set(CONFIG_USE_driver_usdhc true)

#  # description: TRNG Driver
#  set(CONFIG_USE_driver_trng true)

#  # description: SEMA42 Driver
#  set(CONFIG_USE_driver_sema42 true)

#  # description: SCT Driver
#  set(CONFIG_USE_driver_sctimer true)

#  # description: PUF Driver
#  set(CONFIG_USE_driver_puf true)

#  # description: POWERQUAD Driver
#  set(CONFIG_USE_driver_powerquad true)

#  # description: CMSIS DSP functions based on POWERQUAD
#  set(CONFIG_USE_driver_powerquad_cmsis true)

#  # description: PINT Driver
#  set(CONFIG_USE_driver_pint true)

#  # description: OTFAD Driver
#  set(CONFIG_USE_driver_otfad true)

#  # description: OSTimer Driver
#  set(CONFIG_USE_driver_ostimer true)

#  # description: MU Driver
#  set(CONFIG_USE_driver_mu true)

#  # description: MRT Driver
#  set(CONFIG_USE_driver_mrt true)

#  # description: RTC Driver
#  set(CONFIG_USE_driver_lpc_rtc true)

#  # description: iopctl Driver
#  set(CONFIG_USE_driver_lpc_iopctl true)

#  # description: GPIO Driver
#  set(CONFIG_USE_driver_lpc_gpio true)

#  # description: DMA Driver
#  set(CONFIG_USE_driver_lpc_dma true)

#  # description: CRC Driver
#  set(CONFIG_USE_driver_lpc_crc true)

#  # description: LPADC Driver
#  set(CONFIG_USE_driver_lpadc true)

#  # description: INPUTMUX Driver
#  set(CONFIG_USE_driver_inputmux true)

#  # description: I3C Driver
#  set(CONFIG_USE_driver_i3c true)

#  # description: I3C DMA Driver
#  set(CONFIG_USE_driver_i3c_dma true)

#  # description: Hashcrypt Driver
#  set(CONFIG_USE_driver_hashcrypt true)

#  # description: FMEAS Driver
#  set(CONFIG_USE_driver_fmeas true)

#  # description: FLEXSPI Driver
#  set(CONFIG_USE_driver_flexspi true)

#  # description: FLEXSPI DMA Driver
#  set(CONFIG_USE_driver_flexspi_dma true)

#  # description: usart Driver
#  set(CONFIG_USE_driver_flexcomm_usart true)

#  # description: usart Driver
#  set(CONFIG_USE_driver_flexcomm_usart_dma true)

#  # description: SPI Driver
#  set(CONFIG_USE_driver_flexcomm_spi true)

#  # description: SPI Driver
#  set(CONFIG_USE_driver_flexcomm_spi_dma true)

#  # description: IAP3 Driver
#  set(CONFIG_USE_driver_iap3 true)

#  # description: i2s Driver
#  set(CONFIG_USE_driver_flexcomm_i2s true)

#  # description: i2s Driver
#  set(CONFIG_USE_driver_flexcomm_i2s_dma true)

#  # description: I2C Driver
#  set(CONFIG_USE_driver_flexcomm_i2c true)

#  # description: I2C Driver
#  set(CONFIG_USE_driver_flexcomm_i2c_dma true)

#  # description: FLEXCOMM Driver
#  set(CONFIG_USE_driver_flexcomm true)

#  # description: SPI CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_flexcomm_spi true)

#  # description: I2C CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_flexcomm_i2c true)

#  # description: USART CMSIS Driver
#  set(CONFIG_USE_driver_cmsis_flexcomm_usart true)

#  # description: DMIC Driver
#  set(CONFIG_USE_driver_dmic true)

#  # description: DMIC DMA Driver
#  set(CONFIG_USE_driver_dmic_dma true)

#  # description: DMIC HWVAD Driver
#  set(CONFIG_USE_driver_dmic_hwvad true)

#  # description: CTimer Driver
#  set(CONFIG_USE_driver_ctimer true)

#  # description: CASPER Driver
#  set(CONFIG_USE_driver_casper true)

#  # description: CACHE Driver
#  set(CONFIG_USE_driver_cache_cache64 true)

#  # description: ACMP Driver
#  set(CONFIG_USE_driver_acmp true)

#  # description: Component usart_adapter
#  set(CONFIG_USE_component_usart_adapter true)

#  # description: Driver ft6x06
#  set(CONFIG_USE_driver_ft6x06 true)

#  # description: Component timer_manager
#  set(CONFIG_USE_component_timer_manager true)

#  # description: Component ctimer_adapter
#  set(CONFIG_USE_component_ctimer_adapter true)

#  # description: Component mrt_adapter
#  set(CONFIG_USE_component_mrt_adapter true)

#  # description: Component ostimer_adapter
#  set(CONFIG_USE_component_ostimer_adapter true)

#  # description: Driver sx1502
#  set(CONFIG_USE_driver_sx1502 true)

#  # description: Component flexcomm_spi_adapter
#  set(CONFIG_USE_component_flexcomm_spi_adapter true)

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

#  # description: Component lpcrtc
#  set(CONFIG_USE_component_lpcrtc true)

#  # description: Component trng_adapter
#  set(CONFIG_USE_component_trng_adapter true)

#  # description: Component pwm_ctimer_adapter
#  set(CONFIG_USE_component_pwm_ctimer_adapter true)

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

#  # description: Component messaging
#  set(CONFIG_USE_component_messaging true)

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

#  # description: Component flash_adapter
#  set(CONFIG_USE_component_flash_adapter true)

#  # description: Component flexspi_nor_flash_adapter_rt685evk
#  set(CONFIG_USE_component_flexspi_nor_flash_adapter_rt685evk true)

#  # description: Component i3c_bus
#  set(CONFIG_USE_component_i3c_bus true)

#  # description: Component i3c_bus_adapter
#  set(CONFIG_USE_component_i3c_bus_adapter true)

#  # description: Component flexcomm_i2c_adapter
#  set(CONFIG_USE_component_flexcomm_i2c_adapter true)

#  # description: Component i3c_adapter
#  set(CONFIG_USE_component_i3c_adapter true)

#  # description: Component i2c_adapter_interface
#  set(CONFIG_USE_component_i2c_adapter_interface true)

#  # description: Component lpc_gpio_adapter
#  set(CONFIG_USE_component_lpc_gpio_adapter true)

#  # description: Component rt_gpio_adapter
#  set(CONFIG_USE_component_rt_gpio_adapter true)

#  # description: Component.flash_nand_flexspi
#  set(CONFIG_USE_component_flash_nand_flexspi true)

#  # description: Component flash_nor_flexspi
#  set(CONFIG_USE_component_flash_nor_flexspi true)

#  # description: mflash offchip
#  set(CONFIG_USE_component_mflash_offchip true)

#  # description: Driver ili9341
#  set(CONFIG_USE_driver_ili9341 true)

#  # description: Driver psp27801
#  set(CONFIG_USE_driver_psp27801 true)

#  # description: Component lpc_crc_adapter
#  set(CONFIG_USE_component_lpc_crc_adapter true)

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

#  # description: Component flexcomm_i2s_dma_adapter
#  set(CONFIG_USE_component_audio_flexcomm_i2s_dma_adapter true)

#  # description: Device MIMXRT685S_cmsis
#  set(CONFIG_USE_device_CMSIS true)

#  # description: Device MIMXRT685S_system
#  set(CONFIG_USE_device_system true)

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

#  # description: transport_mbedtls, IoT reference common example component
#  set(CONFIG_USE_middleware_iot_reference_transport_mbedtls true)

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

#set.middleware.tinycbor
#  # description: Concise Binary Object Representation (CBOR) Library
#  set(CONFIG_USE_middleware_tinycbor true)

#set.middleware.tfm
#  # description: TFM CMSIS driver for LPUART based on USART
#  set(CONFIG_USE_middleware_tfm_lpuart true)

#  # description: CQBOR library
#  set(CONFIG_USE_middleware_tfm_qcbor true)

#  # description: CQBOR library tests
#  set(CONFIG_USE_middleware_tfm_qcbor_test true)

#  # description: TFM Secure Storage Service
#  set(CONFIG_USE_middleware_tfm_s_ps true)

#  # description: TFM Test PS Secure
#  set(CONFIG_USE_middleware_tfm_s_test_ps true)

#  # description: TFM Test ITS Secure
#  set(CONFIG_USE_middleware_tfm_s_test_its true)

#  # description: TFM Test Attestation Secure
#  set(CONFIG_USE_middleware_tfm_s_test_attestation true)

#  # description: TFM t_cose sign1 verify
#  set(CONFIG_USE_middleware_tfm_t_cose_sign1_verify true)

#  # description: TFM t_cose mac0 verify
#  set(CONFIG_USE_middleware_tfm_t_cose_mac0_verify true)

#  # description: TFM Test Crypto Secure
#  set(CONFIG_USE_middleware_tfm_s_test_crypto true)

#  # description: TFM Platform test
#  set(CONFIG_USE_middleware_tfm_platform_test true)

#  # description: TFM Test Platform Secure
#  set(CONFIG_USE_middleware_tfm_s_test_platform true)

#  # description: TFM Test IPC Secure
#  set(CONFIG_USE_middleware_tfm_s_test_ipc true)

#  # description: TFM Test PS Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_ps true)

#  # description: TFM Test ITS Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_its true)

#  # description: TFM Test Secure Framework
#  set(CONFIG_USE_middleware_tfm_ns_test_s true)

#  # description: TFM Tests Attestation Non Secure
#  set(CONFIG_USE_middleware_tfmtests_ns_test_attestation true)

#  # description: TFM Test Crypto Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_crypto true)

#  # description: TFM Test QCBOR Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_qcbor true)

#  # description: TFM Test IPC Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_ipc true)

#  # description: TFM Test Platform Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_platform true)

#  # description: TFM Tests T_COSE Non Secure
#  set(CONFIG_USE_middleware_tfmtests_ns_test_tcose true)

#  # description: TFM Library
#  set(CONFIG_USE_middleware_tfm_s_platform_common true)

#  # description: TFM CMSIS USART driver
#  set(CONFIG_USE_middleware_tfm_usart true)

#  # description: TFM Platform Service
#  set(CONFIG_USE_middleware_tfm_s_platform true)

#  # description: TFM Platform Service for RW61x
#  set(CONFIG_USE_middleware_tfm_s_platform_sp_common true)

#  # description: TFM Initial Attestation Service asymmetric
#  set(CONFIG_USE_middleware_tfm_s_initial_attestation_asymmetric true)

#  # description: TFM Initial Attestation Service asymmetric
#  set(CONFIG_USE_middleware_tfm_s_initial_attestation_symmetric true)

#  # description: TFM Initial Attestation Service
#  set(CONFIG_USE_middleware_tfm_s_initial_attestation true)

#  # description: TFM Internal Trusted Storage Service
#  set(CONFIG_USE_middleware_tfm_s_its true)

#  # description: TFM Internal Trusted Storage Service Load Info
#  set(CONFIG_USE_middleware_tfm_s_its_load_info_common true)

#  # description: TFM Non Secure
#  set(CONFIG_USE_middleware_tfm_ns true)

#  # description: TFM NS Large Profile
#  set(CONFIG_USE_middleware_tfm_profile_large_ns_configs true)

#  # description: TFM NS Medium Profile
#  set(CONFIG_USE_middleware_tfm_profile_medium_ns_configs true)

#  # description: TFM NS Small Profile
#  set(CONFIG_USE_middleware_tfm_profile_small_ns_configs true)

#  # description: TFM NS mbedTLS compatibility header
#  set(CONFIG_USE_middleware_tfm_ns_mbedtls_compatibility true)

#  # description: TFM NS OS wrapper for bare metal
#  set(CONFIG_USE_middleware_tfm_ns_os_wrapper_baremetal true)

#  # description: TFM NS OS wrapper for RTOS
#  set(CONFIG_USE_middleware_tfm_ns_os_wrapper_rtos true)

#  # description: TFM Tests Non Secure
#  set(CONFIG_USE_middleware_tfmtests_ns true)

#  # description: TFM Test Secure Framework
#  set(CONFIG_USE_middleware_tfm_s_test true)

#  # description: TFM Framework profile large
#  set(CONFIG_USE_middleware_tfm_profile_large true)

#  # description: TFM Framework profile medium
#  set(CONFIG_USE_middleware_tfm_profile_medium true)

#  # description: TFM Framework profile small
#  set(CONFIG_USE_middleware_tfm_profile_small true)

#  # description: TFM Test Secure Framework profile large
#  set(CONFIG_USE_middleware_tfm_profile_large_s_test_configs true)

#  # description: TFM Test Secure Framework profile medium
#  set(CONFIG_USE_middleware_tfm_profile_medium_s_test_configs true)

#  # description: TFM Test Secure Framework profile small
#  set(CONFIG_USE_middleware_tfm_profile_small_s_test_configs true)

#  # description: TFM Test Attestation Secure Asymmetric
#  set(CONFIG_USE_middleware_tfm_s_test_attestation_asymmetric true)

#  # description: TFM Test Attestation Secure Symmetric
#  set(CONFIG_USE_middleware_tfm_s_test_attestation_symmetric true)

#  # description: TFM Test IRQ Service
#  set(CONFIG_USE_middleware_tfm_s_test_irq_slih true)

#  # description: TFM Test IRQ Service
#  set(CONFIG_USE_middleware_tfm_s_test_irq_flih true)

#  # description: TFM Test Non Secure Framework
#  set(CONFIG_USE_middleware_tfm_ns_test true)

#  # description: TFM Test Non Secure Framework profile large
#  set(CONFIG_USE_middleware_tfm_profile_large_ns_test_configs true)

#  # description: TFM Test Non Secure Framework profile medium
#  set(CONFIG_USE_middleware_tfm_profile_medium_ns_test_configs true)

#  # description: TFM Test Non Secure Framework profile small
#  set(CONFIG_USE_middleware_tfm_profile_small_ns_test_configs true)

#  # description: TFM Test Attestation Non Secure asymmetric
#  set(CONFIG_USE_middleware_tfm_ns_test_attestation_asymmetric true)

#  # description: TFM Test Attestation Non Secure asymmetric
#  set(CONFIG_USE_middleware_tfm_ns_test_attestation_symmetric true)

#  # description: TFM Test Attestation Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_attestation true)

#  # description: TFM Tests Attestation Non Secure asymmetric
#  set(CONFIG_USE_middleware_tfmtests_ns_test_attestation_asymmetric true)

#  # description: TFM Tests Attestation Non Secure Symmetric
#  set(CONFIG_USE_middleware_tfmtests_ns_test_attestation_symmetric true)

#  # description: TFM Test IRQ SLIH Mode Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_irq_slih true)

#  # description: TFM Test IRQ FLIH Mode Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_irq_flih true)

#  # description: TFM Test T_COSE Non Secure
#  set(CONFIG_USE_middleware_tfm_ns_test_tcose true)

#set.middleware.mbedcrypto
#  # description: mbedTLS port library common files for MCUX
#  set(CONFIG_USE_middleware_mbedcrypto_port_config true)

#  # description: mbedTLS port library for entropy for all RNG and TRNG based devices
#  set(CONFIG_USE_middleware_mbedcrypto_port_rng true)

#  # description: Mbed Crypto library
#  set(CONFIG_USE_middleware_mbedcrypto true)

#  # description: Mbed Crypto library HW port
#  set(CONFIG_USE_middleware_mbedcrypto_port true)

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

#  # description: PSA Test Suite Non Secure Large Profile
#  set(CONFIG_USE_middleware_tfm_profile_large_ns_psatest_configs true)

#  # description: PSA Test Suite Non Secure Medium Profile
#  set(CONFIG_USE_middleware_tfm_profile_medium_ns_psatest_configs true)

#  # description: PSA Test Suite Non Secure Small Profile
#  set(CONFIG_USE_middleware_tfm_profile_small_ns_psatest_configs true)

#  # description: PSA Test Suite Initial Attestation Non Secure part 1
#  set(CONFIG_USE_middleware_psatest_ns_initial_attestation true)

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

#  # description: RPMsg-Lite for evkmimxrt685 application
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_evkmimxrt685 true)

#  # description: RPMsg-Lite for evkmimxrt685 hifi4 application
#  set(CONFIG_USE_middleware_multicore_rpmsg-lite_evkmimxrt685_hifi4 true)

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

#  # description: mbedTLS port library for entropy for all RNG and TRNG based devices
#  set(CONFIG_USE_middleware_mbedtls3x_port_rng true)

#  # description: mbedTLS port library for use in TFM NS
#  set(CONFIG_USE_middleware_mbedtls3x_port_tfm true)

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

#set.middleware.mbedtls
#  # description: mbedTLS test suite
#  set(CONFIG_USE_middleware_mbedtls_tests true)

#  # description: mbedTLS 3rdparty code
#  set(CONFIG_USE_middleware_mbedtls_3rdparty true)

#  # description: mbedTLS port library for KPSDK
#  set(CONFIG_USE_middleware_mbedtls_port_ksdk true)

#  # description: mbedTLS library
#  set(CONFIG_USE_middleware_mbedtls true)

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
#  set(CONFIG_USE_middleware_cadence_nnlib_rt600_hifi4 true)

#set.middleware.cjson
#  # description: CJSON library
#  set(CONFIG_USE_middleware_cjson true)

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

#  # description: OS wrapper for running FreeRTOS inside TF-M non secure world
#  set(CONFIG_USE_middleware_freertos-kernel_tfm_ns true)

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

#set.middleware.wireless.wpa_supplicant
#  # description: Wpa supplicant rtos
#  set(CONFIG_USE_middleware_wireless_wpa_supplicant_rtos true)

#  # description: Wpa supplicant rtos hostapd
#  set(CONFIG_USE_middleware_wireless_wpa_supplicant_rtos_hostapd true)

#set.middleware.audio_voice.components.voice_seeker
#  # description: Voice seeker library for HiFi4
#  set(CONFIG_USE_middleware_audio_voice_components_voice_seeker_hifi4 true)

#  # description: Voice seeker memory utilities for RT685_HiFi4
#  set(CONFIG_USE_middleware_audio_voice_components_voice_seeker_rdsp_utilities_public_hifi4 true)

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

#  # description: Middleware usb device ip3511hs
#  set(CONFIG_USE_middleware_usb_device_ip3511hs true)

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

#  # description: Middleware usb host ip3516hs
#  set(CONFIG_USE_middleware_usb_host_ip3516hs true)

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

#  # description: lwIP HTTPS Server Implementation
#  set(CONFIG_USE_middleware_lwip_apps_httpssrv true)

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
#  set(CONFIG_USE_middleware_cadence_naturedsp_hifi4 true)

#set.middleware.cadence.multicore_xaf
#  # description: DSP Audio Streamer
#  set(CONFIG_USE_middleware_cadence_multicore_xaf true)

#set.middleware.cadence.codecs_hifi4
#  # description: DSP Audio Streamer HiFi4 codecs
#  set(CONFIG_USE_middleware_cadence_codecs_hifi4 true)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Core/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/DSP
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Driver
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/NN
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2
  ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/RTOS2/Include
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/evkmimxrt685/flash_config
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/evkmimxrt685/project_template
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/mimxrt685audevk/flash_config
  ${CMAKE_CURRENT_LIST_DIR}/../../boards/mimxrt685audevk/project_template
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
  ${CMAKE_CURRENT_LIST_DIR}/../../components/crc
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/ili9341
  ${CMAKE_CURRENT_LIST_DIR}/../../components/display/psp27801
  ${CMAKE_CURRENT_LIST_DIR}/../../components/edgefast_wifi
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/mflash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/gpio
  ${CMAKE_CURRENT_LIST_DIR}/../../components/i2c
  ${CMAKE_CURRENT_LIST_DIR}/../../components/i3c_bus
  ${CMAKE_CURRENT_LIST_DIR}/../../components/internal_flash
  ${CMAKE_CURRENT_LIST_DIR}/../../components/led
  ${CMAKE_CURRENT_LIST_DIR}/../../components/lists
  ${CMAKE_CURRENT_LIST_DIR}/../../components/log
  ${CMAKE_CURRENT_LIST_DIR}/../../components/mem_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/messaging
  ${CMAKE_CURRENT_LIST_DIR}/../../components/mpi_loader
  ${CMAKE_CURRENT_LIST_DIR}/../../components/osa
  ${CMAKE_CURRENT_LIST_DIR}/../../components/panic
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pca9420
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pca9422
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf1550
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf3000
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pmic/pf5020
  ${CMAKE_CURRENT_LIST_DIR}/../../components/power_manager/core
  ${CMAKE_CURRENT_LIST_DIR}/../../components/pwm
  ${CMAKE_CURRENT_LIST_DIR}/../../components/reset
  ${CMAKE_CURRENT_LIST_DIR}/../../components/rng
  ${CMAKE_CURRENT_LIST_DIR}/../../components/rtc
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
  ${CMAKE_CURRENT_LIST_DIR}/../../components/sx1502
  ${CMAKE_CURRENT_LIST_DIR}/../../components/timer
  ${CMAKE_CURRENT_LIST_DIR}/../../components/timer_manager
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ektf2k
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ft3267
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ft5406
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/ft6x06
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/gt911
  ${CMAKE_CURRENT_LIST_DIR}/../../components/touch/tma525b
  ${CMAKE_CURRENT_LIST_DIR}/../../components/uart
  ${CMAKE_CURRENT_LIST_DIR}/../../components/wifi_bt_module
  ${CMAKE_CURRENT_LIST_DIR}/../../components/wifi_bt_module/template
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/audio_voice/components/vit
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/audio_voice/components/voice_seeker/RT685_HiFi4
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/audio_voice/components/voice_seeker/RT685_HiFi4/rdsp_utilities_public
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/device-defender
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/device-shadow
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/jobs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/aws_iot/ota
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/codecs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/multicore-xaf
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/naturedsp/hifi4
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cadence/nnlib
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/cjson
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/edgefast_bluetooth
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/edgefast_bluetooth/lib
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/emwin
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/fatfs
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/freemaster
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
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mbedtls
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mbedtls3x
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/boot/bootutil
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/boot/nxp_mcux_sdk
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/boot/nxp_mcux_sdk/flashapi
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/ext/mbedtls-asn1
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/ext/nxp_encrypted_xip
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/mcuboot_opensource/ext/tinycrypt/lib
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/multicore
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/pkcs11
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/sdmmc
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tfm/mbed-crypto
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tfm/psa-arch-tests/api-tests
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tfm/qcbor
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tfm/tf-m
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tfm/tf-m-tests
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/tinycbor
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/usb
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/wifi_nxp
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/wireless/ethermind
  ${CMAKE_CURRENT_LIST_DIR}/../../middleware/wireless/wpa_supplicant-rtos
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
include_if_use(board_project_template)
include_if_use(board_project_template)
include_if_use(component_ak4497_adapter.MIMXRT685S)
include_if_use(component_audio_flexcomm_i2s_dma_adapter.MIMXRT685S)
include_if_use(component_button.MIMXRT685S)
include_if_use(component_codec_adapters.MIMXRT685S)
include_if_use(component_codec_i2c.MIMXRT685S)
include_if_use(component_common_task)
include_if_use(component_cs42448_adapter.MIMXRT685S)
include_if_use(component_cs42888_adapter.MIMXRT685S)
include_if_use(component_ctimer_adapter.MIMXRT685S)
include_if_use(component_da7212_adapter.MIMXRT685S)
include_if_use(component_flash_adapter.MIMXRT685S)
include_if_use(component_flash_nand_flexspi.MIMXRT685S)
include_if_use(component_flash_nor_flexspi.MIMXRT685S)
include_if_use(component_flexcomm_i2c_adapter.MIMXRT685S)
include_if_use(component_flexcomm_spi_adapter.MIMXRT685S)
include_if_use(component_flexspi_nor_flash_adapter_rt685evk.MIMXRT685S)
include_if_use(component_i2c_adapter_interface.MIMXRT685S)
include_if_use(component_i3c_adapter.MIMXRT685S)
include_if_use(component_i3c_bus.MIMXRT685S)
include_if_use(component_i3c_bus_adapter.MIMXRT685S)
include_if_use(component_led.MIMXRT685S)
include_if_use(component_lists.MIMXRT685S)
include_if_use(component_log.MIMXRT685S)
include_if_use(component_log_backend_debug_console.MIMXRT685S)
include_if_use(component_log_backend_debug_console_lite.MIMXRT685S)
include_if_use(component_log_backend_ringbuffer.MIMXRT685S)
include_if_use(component_lpc_crc_adapter.MIMXRT685S)
include_if_use(component_lpc_gpio_adapter.MIMXRT685S)
include_if_use(component_lpcrtc.MIMXRT685S)
include_if_use(component_mem_manager.MIMXRT685S)
include_if_use(component_mem_manager_freertos.MIMXRT685S)
include_if_use(component_mem_manager_legacy.MIMXRT685S)
include_if_use(component_mem_manager_light.MIMXRT685S)
include_if_use(component_messaging.MIMXRT685S)
include_if_use(component_mflash_offchip.MIMXRT685S)
include_if_use(component_mpi_loader.MIMXRT685S)
include_if_use(component_mrt_adapter.MIMXRT685S)
include_if_use(component_osa)
include_if_use(component_osa_bm)
include_if_use(component_osa_free_rtos)
include_if_use(component_osa_interface)
include_if_use(component_osa_template_config)
include_if_use(component_ostimer_adapter.MIMXRT685S)
include_if_use(component_panic.MIMXRT685S)
include_if_use(component_pcm186x_adapter.MIMXRT685S)
include_if_use(component_pcm512x_adapter.MIMXRT685S)
include_if_use(component_power_manager_framework.MIMXRT685S)
include_if_use(component_pwm_ctimer_adapter.MIMXRT685S)
include_if_use(component_reset_adapter.MIMXRT685S)
include_if_use(component_rt_gpio_adapter.MIMXRT685S)
include_if_use(component_serial_manager.MIMXRT685S)
include_if_use(component_serial_manager_spi.MIMXRT685S)
include_if_use(component_serial_manager_swo.MIMXRT685S)
include_if_use(component_serial_manager_uart.MIMXRT685S)
include_if_use(component_serial_manager_usb_cdc.MIMXRT685S)
include_if_use(component_serial_manager_virtual.MIMXRT685S)
include_if_use(component_sgtl_adapter.MIMXRT685S)
include_if_use(component_software_crc_adapter.MIMXRT685S)
include_if_use(component_software_rng_adapter.MIMXRT685S)
include_if_use(component_tfa9896_adapter.MIMXRT685S)
include_if_use(component_tfa9xxx_adapter.MIMXRT685S)
include_if_use(component_timer_manager.MIMXRT685S)
include_if_use(component_trng_adapter.MIMXRT685S)
include_if_use(component_usart_adapter.MIMXRT685S)
include_if_use(component_wifi_bt_module_config.MIMXRT685S)
include_if_use(component_wifi_bt_module_tx_pwr_limits.MIMXRT685S)
include_if_use(component_wm8524_adapter.MIMXRT685S)
include_if_use(component_wm8904_adapter.MIMXRT685S)
include_if_use(component_wm8960_adapter.MIMXRT685S)
include_if_use(component_wm8962_adapter.MIMXRT685S)
include_if_use(device_CMSIS.MIMXRT685S)
include_if_use(device_RTE.MIMXRT685S)
include_if_use(device_project_template.MIMXRT685S)
include_if_use(device_startup.MIMXRT685S)
include_if_use(device_system.MIMXRT685S)
include_if_use(driver_acmp.MIMXRT685S)
include_if_use(driver_ak4497.MIMXRT685S)
include_if_use(driver_cache_cache64.MIMXRT685S)
include_if_use(driver_casper.MIMXRT685S)
include_if_use(driver_clock.MIMXRT685S)
include_if_use(driver_cmsis_flexcomm_i2c.MIMXRT685S)
include_if_use(driver_cmsis_flexcomm_spi.MIMXRT685S)
include_if_use(driver_cmsis_flexcomm_usart.MIMXRT685S)
include_if_use(driver_codec.MIMXRT685S)
include_if_use(driver_common.MIMXRT685S)
include_if_use(driver_cs42448.MIMXRT685S)
include_if_use(driver_cs42888.MIMXRT685S)
include_if_use(driver_ctimer.MIMXRT685S)
include_if_use(driver_dialog7212.MIMXRT685S)
include_if_use(driver_dmic.MIMXRT685S)
include_if_use(driver_dmic_dma.MIMXRT685S)
include_if_use(driver_dmic_hwvad.MIMXRT685S)
include_if_use(driver_dsp.MIMXRT685S)
include_if_use(driver_ektf2k.MIMXRT685S)
include_if_use(driver_flash_config)
include_if_use(driver_flash_config)
include_if_use(driver_flexcomm.MIMXRT685S)
include_if_use(driver_flexcomm_i2c.MIMXRT685S)
include_if_use(driver_flexcomm_i2c_dma.MIMXRT685S)
include_if_use(driver_flexcomm_i2c_freertos.MIMXRT685S)
include_if_use(driver_flexcomm_i2s.MIMXRT685S)
include_if_use(driver_flexcomm_i2s_dma.MIMXRT685S)
include_if_use(driver_flexcomm_spi.MIMXRT685S)
include_if_use(driver_flexcomm_spi_dma.MIMXRT685S)
include_if_use(driver_flexcomm_spi_freertos.MIMXRT685S)
include_if_use(driver_flexcomm_usart.MIMXRT685S)
include_if_use(driver_flexcomm_usart_dma.MIMXRT685S)
include_if_use(driver_flexcomm_usart_freertos.MIMXRT685S)
include_if_use(driver_flexspi.MIMXRT685S)
include_if_use(driver_flexspi_dma.MIMXRT685S)
include_if_use(driver_fmeas.MIMXRT685S)
include_if_use(driver_ft3267.MIMXRT685S)
include_if_use(driver_ft5406.MIMXRT685S)
include_if_use(driver_ft6x06.MIMXRT685S)
include_if_use(driver_fxas21002cq.MIMXRT685S)
include_if_use(driver_fxls8974cf.MIMXRT685S)
include_if_use(driver_fxos8700cq.MIMXRT685S)
include_if_use(driver_gt911.MIMXRT685S)
include_if_use(driver_hashcrypt.MIMXRT685S)
include_if_use(driver_htu21d.MIMXRT685S)
include_if_use(driver_i2s_bridge.MIMXRT685S)
include_if_use(driver_i3c.MIMXRT685S)
include_if_use(driver_i3c_dma.MIMXRT685S)
include_if_use(driver_iap3.MIMXRT685S)
include_if_use(driver_icm42688p.MIMXRT685S)
include_if_use(driver_ili9341.MIMXRT685S)
include_if_use(driver_inputmux.MIMXRT685S)
include_if_use(driver_inputmux_connections.MIMXRT685S)
include_if_use(driver_lpadc.MIMXRT685S)
include_if_use(driver_lpc_crc.MIMXRT685S)
include_if_use(driver_lpc_dma.MIMXRT685S)
include_if_use(driver_lpc_gpio.MIMXRT685S)
include_if_use(driver_lpc_iopctl.MIMXRT685S)
include_if_use(driver_lpc_rtc.MIMXRT685S)
include_if_use(driver_lsm6dso.MIMXRT685S)
include_if_use(driver_max30101.MIMXRT685S)
include_if_use(driver_mma8451q.MIMXRT685S)
include_if_use(driver_mma8652fc.MIMXRT685S)
include_if_use(driver_mrt.MIMXRT685S)
include_if_use(driver_mu.MIMXRT685S)
include_if_use(driver_ostimer.MIMXRT685S)
include_if_use(driver_otfad.MIMXRT685S)
include_if_use(driver_p3t1755.MIMXRT685S)
include_if_use(driver_pca9420.MIMXRT685S)
include_if_use(driver_pca9422.MIMXRT685S)
include_if_use(driver_pcm186x.MIMXRT685S)
include_if_use(driver_pcm512x.MIMXRT685S)
include_if_use(driver_pf1550.MIMXRT685S)
include_if_use(driver_pf3000.MIMXRT685S)
include_if_use(driver_pf5020.MIMXRT685S)
include_if_use(driver_pint.MIMXRT685S)
include_if_use(driver_power.MIMXRT685S)
include_if_use(driver_powerquad.MIMXRT685S)
include_if_use(driver_powerquad_cmsis.MIMXRT685S)
include_if_use(driver_psp27801.MIMXRT685S)
include_if_use(driver_puf.MIMXRT685S)
include_if_use(driver_reset.MIMXRT685S)
include_if_use(driver_sctimer.MIMXRT685S)
include_if_use(driver_sema42.MIMXRT685S)
include_if_use(driver_sgtl5000.MIMXRT685S)
include_if_use(driver_sx1502.MIMXRT685S)
include_if_use(driver_tfa9896.MIMXRT685S)
include_if_use(driver_tfa9xxx.MIMXRT685S)
include_if_use(driver_tfa9xxx_hal.MIMXRT685S)
include_if_use(driver_tma525b.MIMXRT685S)
include_if_use(driver_trng.MIMXRT685S)
include_if_use(driver_tsl2561.MIMXRT685S)
include_if_use(driver_usdhc.MIMXRT685S)
include_if_use(driver_utick.MIMXRT685S)
include_if_use(driver_wm8524.MIMXRT685S)
include_if_use(driver_wm8904.MIMXRT685S)
include_if_use(driver_wm8960.MIMXRT685S)
include_if_use(driver_wm8962.MIMXRT685S)
include_if_use(driver_wwdt.MIMXRT685S)
include_if_use(middleware_audio_voice_components_vit_hifi4)
include_if_use(middleware_audio_voice_components_vit_hifi4_models)
include_if_use(middleware_audio_voice_components_voice_seeker_hifi4)
include_if_use(middleware_audio_voice_components_voice_seeker_rdsp_utilities_public_hifi4)
include_if_use(middleware_aws_iot_device_defender)
include_if_use(middleware_aws_iot_device_shadow)
include_if_use(middleware_aws_iot_device_shadow_template)
include_if_use(middleware_aws_iot_jobs)
include_if_use(middleware_aws_iot_ota)
include_if_use(middleware_aws_iot_ota_freertos)
include_if_use(middleware_cadence_codecs_hifi4)
include_if_use(middleware_cadence_multicore_xaf)
include_if_use(middleware_cadence_naturedsp_hifi4)
include_if_use(middleware_cadence_nnlib)
include_if_use(middleware_cadence_nnlib_rt600_hifi4)
include_if_use(middleware_cjson)
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
include_if_use(middleware_freertos-kernel_tfm_ns)
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
include_if_use(middleware_iot_reference_transport_mbedtls)
include_if_use(middleware_iot_reference_transport_mbedtls3x)
include_if_use(middleware_iot_reference_transport_mbedtls_psa)
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
include_if_use(middleware_lwip_apps_httpssrv)
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
include_if_use(middleware_mbedcrypto)
include_if_use(middleware_mbedcrypto_port)
include_if_use(middleware_mbedcrypto_port_config)
include_if_use(middleware_mbedcrypto_port_rng)
include_if_use(middleware_mbedtls)
include_if_use(middleware_mbedtls3x)
include_if_use(middleware_mbedtls3x_crypto)
include_if_use(middleware_mbedtls3x_crypto_no_psa)
include_if_use(middleware_mbedtls3x_crypto_storage_default)
include_if_use(middleware_mbedtls3x_crypto_storage_ram)
include_if_use(middleware_mbedtls3x_no_psa)
include_if_use(middleware_mbedtls3x_port_config)
include_if_use(middleware_mbedtls3x_port_hw_init)
include_if_use(middleware_mbedtls3x_port_psa_crypto_config)
include_if_use(middleware_mbedtls3x_port_rng)
include_if_use(middleware_mbedtls3x_port_tfm)
include_if_use(middleware_mbedtls3x_ssl)
include_if_use(middleware_mbedtls3x_ssl_no_psa)
include_if_use(middleware_mbedtls3x_template)
include_if_use(middleware_mbedtls3x_tests)
include_if_use(middleware_mbedtls3x_x509)
include_if_use(middleware_mbedtls_3rdparty)
include_if_use(middleware_mbedtls_port_ksdk)
include_if_use(middleware_mbedtls_tests)
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
include_if_use(middleware_multicore_rpmsg-lite)
include_if_use(middleware_multicore_rpmsg-lite_bm)
include_if_use(middleware_multicore_rpmsg-lite_evkmimxrt685)
include_if_use(middleware_multicore_rpmsg-lite_evkmimxrt685_hifi4)
include_if_use(middleware_multicore_rpmsg-lite_freertos)
include_if_use(middleware_multicore_rpmsg-lite_ns)
include_if_use(middleware_multicore_rpmsg-lite_qnx)
include_if_use(middleware_multicore_rpmsg-lite_queue)
include_if_use(middleware_multicore_rpmsg-lite_threadx)
include_if_use(middleware_multicore_rpmsg-lite_xos)
include_if_use(middleware_multicore_rpmsg-lite_zephyr)
include_if_use(middleware_pkcs11)
include_if_use(middleware_psatest_ns)
include_if_use(middleware_psatest_ns_crypto)
include_if_use(middleware_psatest_ns_initial_attestation)
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
include_if_use(middleware_tfm_lpuart)
include_if_use(middleware_tfm_ns)
include_if_use(middleware_tfm_ns_mbedtls_compatibility)
include_if_use(middleware_tfm_ns_os_wrapper_baremetal)
include_if_use(middleware_tfm_ns_os_wrapper_rtos)
include_if_use(middleware_tfm_ns_test)
include_if_use(middleware_tfm_ns_test_attestation)
include_if_use(middleware_tfm_ns_test_attestation_asymmetric)
include_if_use(middleware_tfm_ns_test_attestation_symmetric)
include_if_use(middleware_tfm_ns_test_crypto)
include_if_use(middleware_tfm_ns_test_ipc)
include_if_use(middleware_tfm_ns_test_irq_flih)
include_if_use(middleware_tfm_ns_test_irq_slih)
include_if_use(middleware_tfm_ns_test_its)
include_if_use(middleware_tfm_ns_test_platform)
include_if_use(middleware_tfm_ns_test_ps)
include_if_use(middleware_tfm_ns_test_qcbor)
include_if_use(middleware_tfm_ns_test_s)
include_if_use(middleware_tfm_ns_test_tcose)
include_if_use(middleware_tfm_platform_test)
include_if_use(middleware_tfm_profile_large)
include_if_use(middleware_tfm_profile_large_ns_configs)
include_if_use(middleware_tfm_profile_large_ns_psatest_configs)
include_if_use(middleware_tfm_profile_large_ns_test_configs)
include_if_use(middleware_tfm_profile_large_s_test_configs)
include_if_use(middleware_tfm_profile_medium)
include_if_use(middleware_tfm_profile_medium_ns_configs)
include_if_use(middleware_tfm_profile_medium_ns_psatest_configs)
include_if_use(middleware_tfm_profile_medium_ns_test_configs)
include_if_use(middleware_tfm_profile_medium_s_test_configs)
include_if_use(middleware_tfm_profile_small)
include_if_use(middleware_tfm_profile_small_ns_configs)
include_if_use(middleware_tfm_profile_small_ns_psatest_configs)
include_if_use(middleware_tfm_profile_small_ns_test_configs)
include_if_use(middleware_tfm_profile_small_s_test_configs)
include_if_use(middleware_tfm_qcbor)
include_if_use(middleware_tfm_qcbor_test)
include_if_use(middleware_tfm_s_initial_attestation)
include_if_use(middleware_tfm_s_initial_attestation_asymmetric)
include_if_use(middleware_tfm_s_initial_attestation_symmetric)
include_if_use(middleware_tfm_s_its)
include_if_use(middleware_tfm_s_its_load_info_common)
include_if_use(middleware_tfm_s_platform)
include_if_use(middleware_tfm_s_platform_common)
include_if_use(middleware_tfm_s_platform_sp_common)
include_if_use(middleware_tfm_s_ps)
include_if_use(middleware_tfm_s_test)
include_if_use(middleware_tfm_s_test_attestation)
include_if_use(middleware_tfm_s_test_attestation_asymmetric)
include_if_use(middleware_tfm_s_test_attestation_symmetric)
include_if_use(middleware_tfm_s_test_crypto)
include_if_use(middleware_tfm_s_test_ipc)
include_if_use(middleware_tfm_s_test_irq_flih)
include_if_use(middleware_tfm_s_test_irq_slih)
include_if_use(middleware_tfm_s_test_its)
include_if_use(middleware_tfm_s_test_platform)
include_if_use(middleware_tfm_s_test_ps)
include_if_use(middleware_tfm_t_cose_mac0_verify)
include_if_use(middleware_tfm_t_cose_sign1_verify)
include_if_use(middleware_tfm_usart)
include_if_use(middleware_tfmtests_ns)
include_if_use(middleware_tfmtests_ns_test_attestation)
include_if_use(middleware_tfmtests_ns_test_attestation_asymmetric)
include_if_use(middleware_tfmtests_ns_test_attestation_symmetric)
include_if_use(middleware_tfmtests_ns_test_tcose)
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
include_if_use(middleware_usb_device_hid)
include_if_use(middleware_usb_device_ip3511hs)
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
include_if_use(middleware_usb_host_hid)
include_if_use(middleware_usb_host_ip3516hs)
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
include_if_use(middleware_wireless_wpa_supplicant_rtos)
include_if_use(middleware_wireless_wpa_supplicant_rtos_hostapd)
include_if_use(utilities_misc_utilities.MIMXRT685S)
include_if_use(utility_assert.MIMXRT685S)
include_if_use(utility_assert_lite.MIMXRT685S)
include_if_use(utility_debug_console.MIMXRT685S)
include_if_use(utility_debug_console_lite.MIMXRT685S)
include_if_use(utility_debug_console_template_config.MIMXRT685S)
include_if_use(utility_incbin.MIMXRT685S)
include_if_use(utility_shell.MIMXRT685S)
include_if_use(utility_str.MIMXRT685S)
