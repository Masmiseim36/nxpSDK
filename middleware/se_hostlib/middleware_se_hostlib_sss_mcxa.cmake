# Add set(CONFIG_USE_middleware_se_hostlib_sss_mcxa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC
              )
  
  
  endif()

