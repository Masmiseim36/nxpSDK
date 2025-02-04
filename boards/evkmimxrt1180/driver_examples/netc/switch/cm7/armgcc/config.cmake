# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT1189/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_driver_anadig_pmu_1 true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_dcdc_3 true)
set(CONFIG_USE_driver_ele_base_api true)
set(CONFIG_USE_driver_gpc_3 true)
set(CONFIG_USE_driver_iomuxc true)
set(CONFIG_USE_driver_memory true)
set(CONFIG_USE_driver_misc true)
set(CONFIG_USE_driver_src_3 true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_CMSIS true)
set(CONFIG_USE_device_system true)
set(CONFIG_USE_device_startup true)
set(CONFIG_USE_driver_cache_armv7_m7 true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_driver_msgintr true)
set(CONFIG_USE_driver_netc_rt1180 true)
set(CONFIG_USE_driver_netc true)
set(CONFIG_USE_driver_netc_switch true)
set(CONFIG_USE_driver_rgpio true)
set(CONFIG_USE_driver_s3mu true)
set(CONFIG_USE_utility_assert_lite true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_utility_str true)
set(CONFIG_USE_utility_debug_console_lite true)
set(CONFIG_USE_component_lpuart_adapter true)
set(CONFIG_USE_component_eth_phy_common true)
set(CONFIG_USE_component_phyrtl8211f true)
set(CONFIG_USE_component_phyrtl8201 true)
set(CONFIG_USE_component_silicon_id true)
set(CONFIG_USE_component_silicon_id_imxrt1180 true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMXRT1189)
set(CONFIG_BOARD evkmimxrt1180)
set(CONFIG_KIT evkmimxrt1180)
set(CONFIG_DEVICE_ID MIMXRT1189xxxxx)
set(CONFIG_FPU DP_FPU)
set(CONFIG_DSP NO_DSP)
set(CONFIG_CORE_ID cm7)
set(CONFIG_TRUSTZONE NO_TZ)
