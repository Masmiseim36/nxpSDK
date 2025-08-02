# Add set(CONFIG_USE_component_wifi_bt_module_bt_only_fw true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCONFIG_BT_ONLY_DNLD
                        -DCONFIG_BT_IND_DNLD
              )
  
  
  endif()

