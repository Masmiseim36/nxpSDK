# Add set(CONFIG_USE_component_wifi_bt_module_slim true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCONFIG_LWIP_LOW_MEM_FOOTPRINT=1
                        -DCONFIG_FREERTOS_LOW_MEMORY_FOOTPRINT=1
              )
  
  
  endif()

