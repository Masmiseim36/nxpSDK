# Add set(CONFIG_USE_middleware_wireless_framework_function_lib_use_toolchain_mem_function true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DgUseToolchainMemFunc_d=1
              )
  
  
  endif()

