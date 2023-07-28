# Add set(CONFIG_USE_middleware_iot_reference_shadow true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos_coremqtt-agent AND CONFIG_USE_middleware_aws_iot_device_shadow AND CONFIG_USE_middleware_iot_reference_shadow_tasks)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/app_main.c
)

else()

message(SEND_ERROR "middleware_iot_reference_shadow dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
