# Add set(CONFIG_USE_middleware_wireless_framework_zephyr_port_nvs_external_flash true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/zephyr/port/nvs/fwk_nvs_ExternalFlash.c
        )

  
  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DgAppNvsExternalFlash_c=1
                        -DgAppNvsInternalFlash_c=0
              )
  
  
  endif()

