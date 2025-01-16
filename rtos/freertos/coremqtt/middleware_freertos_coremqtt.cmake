# Add set(CONFIG_USE_middleware_freertos_coremqtt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt.c
          ${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_serializer.c
          ${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_state.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/source/include
          ${CMAKE_CURRENT_LIST_DIR}/source/interface
        )

  
