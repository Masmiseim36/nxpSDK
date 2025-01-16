# Add set(CONFIG_USE_middleware_audio_voice_maestro_element_audio_proc_asrc_enable true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  
            if(CONFIG_TOOLCHAIN STREQUAL armgcc)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
            )
      endif()
          if(CONFIG_TOOLCHAIN STREQUAL mcux)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
            )
      endif()
      
  endif()

