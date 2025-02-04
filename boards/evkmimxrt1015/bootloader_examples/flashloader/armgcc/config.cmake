# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT1015/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_device_MIMXRT1015_CMSIS true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_igpio true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_driver_iomuxc true)
set(CONFIG_USE_middleware_mcu-boot_MIMXRT1015_startup true)
set(CONFIG_USE_middleware_mcu-boot_src_bm_usb_hid true)
set(CONFIG_USE_middleware_mcu-boot_bootsources true)
set(CONFIG_USE_middleware_mcu-boot_bootsources_imxrt true)
set(CONFIG_USE_middleware_mcu-boot_MIMXRT1015_sources true)
set(CONFIG_USE_middleware_baremetal true)
set(CONFIG_USE_middleware_usb_device_common_header true)
set(CONFIG_USE_middleware_usb_phy true)
set(CONFIG_USE_middleware_mcu-boot_usb_device_controller_driver true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_middleware_mcu-boot_src_startup true)
set(CONFIG_USE_middleware_mcu-boot_drv_microseconds_pit true)
set(CONFIG_USE_middleware_mcu-boot_mem_flexspi_nor true)
set(CONFIG_USE_middleware_mcu-boot_drv_flexspi_nand true)
set(CONFIG_USE_middleware_mcu-boot_mem_spi_nor_eeprom true)
set(CONFIG_USE_middleware_mcu-boot_drv_ocotp true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_component_osa true)
set(CONFIG_USE_middleware_usb_common_header true)
set(CONFIG_USE_middleware_mcu-boot_drv_flexspi_nor true)
set(CONFIG_USE_middleware_mcu-boot_drv_serial_nor_eeprom true)
set(CONFIG_USE_driver_lpspi true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMXRT1015)
set(CONFIG_BOARD evkmimxrt1015)
set(CONFIG_KIT evkmimxrt1015)
set(CONFIG_DEVICE_ID MIMXRT1015xxxxx)
set(CONFIG_FPU DP_FPU)
set(CONFIG_DSP NO_DSP)
