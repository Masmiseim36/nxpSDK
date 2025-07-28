# Add set(CONFIG_USE_middleware_wifi_slim true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCONFIG_WIFI_SLIM_ROAM=1
                        -DCONFIG_WIFI_SLIM_STA=1
                        -DCONFIG_WIFI_SLIM_UAP=1
                        -DCONFIG_WIFI_SLIM_WMM=1
                        -DCONFIG_WIFI_SLIM_TX_RX_AGGR=1
              )
  
  
  endif()

