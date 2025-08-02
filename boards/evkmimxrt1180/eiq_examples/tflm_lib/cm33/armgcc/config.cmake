# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT1189/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_source true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_cmsis_nn true)
set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_library true)
set(CONFIG_CORE cm33)
set(CONFIG_DEVICE MIMXRT1189)
set(CONFIG_BOARD evkmimxrt1180)
set(CONFIG_KIT evkmimxrt1180)
set(CONFIG_DEVICE_ID MIMXRT1189xxxxx)
set(CONFIG_FPU SP_FPU)
set(CONFIG_DSP DSP)
set(CONFIG_CORE_ID cm33)
set(CONFIG_TRUSTZONE TZ)
