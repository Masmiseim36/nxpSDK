# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT685S/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_driver_flash_config true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_CMSIS true)
set(CONFIG_USE_device_system true)
set(CONFIG_USE_device_startup true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_dsp true)
set(CONFIG_USE_driver_i2s_bridge true)
set(CONFIG_USE_driver_inputmux_connections true)
set(CONFIG_USE_driver_power true)
set(CONFIG_USE_driver_reset true)
set(CONFIG_USE_driver_cache_cache64 true)
set(CONFIG_USE_driver_casper true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_flexcomm true)
set(CONFIG_USE_driver_flexcomm_usart true)
set(CONFIG_USE_driver_flexspi true)
set(CONFIG_USE_driver_hashcrypt true)
set(CONFIG_USE_driver_lpc_gpio true)
set(CONFIG_USE_driver_lpc_iopctl true)
set(CONFIG_USE_driver_trng true)
set(CONFIG_USE_utility_assert true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_utility_str true)
set(CONFIG_USE_utility_debug_console true)
set(CONFIG_USE_component_serial_manager true)
set(CONFIG_USE_component_serial_manager_uart true)
set(CONFIG_USE_component_usart_adapter true)
set(CONFIG_USE_component_mflash_offchip true)
set(CONFIG_USE_middleware_mbedtls_port_ksdk_option true)
set(CONFIG_USE_middleware_mbedtls true)
set(CONFIG_USE_middleware_mbedtls_port_ksdk true)
set(CONFIG_USE_middleware_mcuboot_bootutil true)
set(CONFIG_USE_middleware_mcuboot_nxp_bootutil_port true)
set(CONFIG_USE_middleware_mcuboot_stack_heap_default_mbedtls true)
set(CONFIG_USE_middleware_mcuboot_bootutil_flashapi_rt true)
set(CONFIG_CORE cm33)
set(CONFIG_DEVICE MIMXRT685S)
set(CONFIG_BOARD evkmimxrt685)
set(CONFIG_KIT evkmimxrt685)
set(CONFIG_DEVICE_ID MIMXRT685S)
set(CONFIG_FPU SP_FPU)
set(CONFIG_DSP DSP)
set(CONFIG_CORE_ID cm33)
set(CONFIG_TRUSTZONE TZ)
