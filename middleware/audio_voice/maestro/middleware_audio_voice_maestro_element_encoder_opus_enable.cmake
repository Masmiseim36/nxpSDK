# Add set(CONFIG_USE_middleware_audio_voice_maestro_element_encoder_opus_enable true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/components/encoders/opus/opusenc_cei.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/components/encoders/opus
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DOPUS_ENCODE=1
                        -DSTREAMER_ENABLE_CEI_OPUS=1
              )
  
  
  endif()

