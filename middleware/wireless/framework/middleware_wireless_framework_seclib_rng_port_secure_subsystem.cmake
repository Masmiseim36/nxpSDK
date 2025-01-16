# Add set(CONFIG_USE_middleware_wireless_framework_seclib_rng_port_secure_subsystem true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/services/SecLib_RNG/SecLib_sss.c
          ${CMAKE_CURRENT_LIST_DIR}/services/SecLib_RNG/RNG.c
        )

  
  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DgRngUseSecureSubSystem_d=1
              )
  
  
  endif()

