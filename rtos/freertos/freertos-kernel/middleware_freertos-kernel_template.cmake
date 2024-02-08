# Add set(CONFIG_USE_middleware_freertos-kernel_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1181xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1182xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx))
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/ARM_CM33_3_priority_bits/FreeRTOSConfig.h ${CMAKE_CURRENT_LIST_DIR}/template/ARM_CM4F_4_priority_bits middleware_freertos-kernel_template)
endif()

if(CONFIG_CORE STREQUAL cm7f AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx))
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/ARM_CM4F_4_priority_bits/FreeRTOSConfig.h ${CMAKE_CURRENT_LIST_DIR}/template/ARM_CM4F_4_priority_bits middleware_freertos-kernel_template)
endif()

