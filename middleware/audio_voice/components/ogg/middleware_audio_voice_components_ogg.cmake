# Add set(CONFIG_USE_middleware_audio_voice_components_ogg true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/framing.c
          ${CMAKE_CURRENT_LIST_DIR}/src/memory.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/src
          ${CMAKE_CURRENT_LIST_DIR}/include/ogg
        )

  
