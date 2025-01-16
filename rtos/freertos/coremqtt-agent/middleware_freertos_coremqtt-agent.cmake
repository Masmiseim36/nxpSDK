# Add set(CONFIG_USE_middleware_freertos_coremqtt-agent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_agent_command_functions.c
          ${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_agent.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/source/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DMQTT_AGENT_DO_NOT_USE_CUSTOM_CONFIG
              )
  
  
  endif()

