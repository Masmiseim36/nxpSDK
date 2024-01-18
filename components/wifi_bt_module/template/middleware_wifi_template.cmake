# Add set(CONFIG_USE_middleware_wifi_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_wifi_bt_module_tx_pwr_limits AND CONFIG_USE_component_wifi_bt_module_config)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/app_config.h ${CMAKE_CURRENT_LIST_DIR}/. middleware_wifi_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_config.h ${CMAKE_CURRENT_LIST_DIR}/. middleware_wifi_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_bt_config.h ${CMAKE_CURRENT_LIST_DIR}/. middleware_wifi_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_bt_config.c "" middleware_wifi_template)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLWIP_DNS=1
    -DLWIP_NETIF_HOSTNAME=1
    -DLWIP_NETIF_STATUS_CALLBACK=1
    -DLWIP_IGMP=1
    -DSDIO_ENABLED
  )

endif()

else()

message(SEND_ERROR "middleware_wifi_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
