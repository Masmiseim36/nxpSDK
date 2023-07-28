# Add set(CONFIG_USE_middleware_iot_reference_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel AND CONFIG_USE_middleware_iot_reference_logging)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/demo_config.h ${CMAKE_CURRENT_LIST_DIR}/template middleware_iot_reference_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/kvstore_config.h ${CMAKE_CURRENT_LIST_DIR}/template middleware_iot_reference_template)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLWIP_DNS=1
  )

endif()

else()

message(SEND_ERROR "middleware_iot_reference_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
