# Add set(CONFIG_USE_middleware_wifi_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_USE_component_wifi_bt_module_tx_pwr_limits AND CONFIG_USE_driver_imu) OR (CONFIG_USE_component_wifi_bt_module_tx_pwr_limits AND CONFIG_USE_component_wifi_bt_module_config AND CONFIG_USE_middleware_wifi_wifi_bt_config_template AND (NOT CONFIG_USE_driver_imu)))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/app_config.h ${CMAKE_CURRENT_LIST_DIR}/. middleware_wifi_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_config.h ${CMAKE_CURRENT_LIST_DIR}/. middleware_wifi_template)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DOSA_USED
    -DFSL_OSA_TASK_ENABLE=1
    -DLWIP_DNS=1
    -DLWIP_NETIF_HOSTNAME=1
    -DLWIP_IGMP=1
    -D_XOPEN_SOURCE=500
  )

endif()

else()

message(SEND_ERROR "middleware_wifi_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
