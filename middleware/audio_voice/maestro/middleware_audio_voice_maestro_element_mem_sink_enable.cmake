# Add set(CONFIG_USE_middleware_audio_voice_maestro_element_mem_sink_enable true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/elements/mem_sink.c
        )

  
  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DSTREAMER_ENABLE_MEM_SINK
              )
  
  
  endif()

