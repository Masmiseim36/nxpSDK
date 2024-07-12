# Add set(CONFIG_USE_middleware_wifi_wifi_bt_config_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_bt_config.h ${CMAKE_CURRENT_LIST_DIR}/. middleware_wifi_wifi_bt_config_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_bt_config.c "" middleware_wifi_wifi_bt_config_template)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSDIO_ENABLED
  )

endif()

