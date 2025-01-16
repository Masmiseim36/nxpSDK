# Add set(CONFIG_USE_middleware_lwip_apps_httpssrv true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DMBEDTLS_THREADING_C
                        -DMBEDTLS_THREADING_ALT
              )
  
  
  endif()

