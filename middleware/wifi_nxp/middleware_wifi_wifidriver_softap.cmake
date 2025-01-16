# Add set(CONFIG_USE_middleware_wifi_wifidriver_softap true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver/wifi-uap.c
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_uap_ioctl.c
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver/mlan_uap_cmdevent.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/wifidriver
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCONFIG_NXP_WIFI_SOFTAP_SUPPORT=1
              )
  
  
  endif()

