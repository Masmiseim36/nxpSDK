# Add set(CONFIG_USE_middleware_wireless_framework_nvm_interval_policy_wo_jitter true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DgNvmSaveOnIdlePolicy_d=1
              )
  
  
  endif()

