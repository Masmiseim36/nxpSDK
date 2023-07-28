# Add set(CONFIG_USE_device_MIMXRT1052_CMSIS true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_CMSIS_Include_core_cm AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1052xxxxB))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "device_MIMXRT1052_CMSIS.MIMXRT1052 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
