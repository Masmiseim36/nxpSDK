# Add set(CONFIG_USE_middleware_iot_reference_shadow_tasks true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos_coremqtt AND CONFIG_USE_middleware_aws_iot_device_shadow AND CONFIG_USE_middleware_freertos_corejson)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/shadow_device_task.c
  ${CMAKE_CURRENT_LIST_DIR}/shadow_update_task.c
)

else()

message(SEND_ERROR "middleware_iot_reference_shadow_tasks dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
