# Add set(CONFIG_USE_component_wifi_bt_module_wifi_bt_combo_fw true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCONFIG_WIFI_BT_CMOBO_DNLD
                        -DCONFIG_WIFI_IND_DNLD=0
                        -DCONFIG_WIFI_IND_RESET=0
              )
  
  
  endif()

