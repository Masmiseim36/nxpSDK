# Add set(CONFIG_USE_middleware_wifi_wifi_bt_config_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

            add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_bt_config.h "" middleware_wifi_wifi_bt_config_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/wifi_bt_config.c "" middleware_wifi_wifi_bt_config_template)
    
  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../../../middleware/wifi_nxp
        )

  
