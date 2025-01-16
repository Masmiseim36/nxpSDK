# Add set(CONFIG_USE_middleware_audio_voice_maestro_element_decoder_mp3_enable true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL iar OR CONFIG_TOOLCHAIN STREQUAL mcux))
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/components/decoders/mp3/mp3_extractmetadata.c
          ${CMAKE_CURRENT_LIST_DIR}/components/decoders/mp3/mp3_id3v2.c
          ${CMAKE_CURRENT_LIST_DIR}/components/decoders/mp3/mp3_vbr.c
        )
    endif()

  
      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL iar OR CONFIG_TOOLCHAIN STREQUAL mcux))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/components/decoders/mp3
        )
    endif()

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  
            if(CONFIG_TOOLCHAIN STREQUAL armgcc)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
            )
      endif()
          if(CONFIG_TOOLCHAIN STREQUAL iar)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
            )
      endif()
          if(CONFIG_TOOLCHAIN STREQUAL mcux)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
            )
      endif()
      
  endif()

