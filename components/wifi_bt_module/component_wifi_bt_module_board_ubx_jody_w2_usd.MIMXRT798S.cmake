# Add set(CONFIG_USE_component_wifi_bt_module_board_ubx_jody_w2_usd true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DWIFI_88W8987_BOARD_UBX_JODY_W2_USD
              )
  
  
  endif()

