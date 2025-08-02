# Add set(CONFIG_USE_component_wifi_bt_module_board_aw_am510_arduino true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DWIFI_IW416_BOARD_AW_AM510_ARDUINO
              )
  
  
  endif()

