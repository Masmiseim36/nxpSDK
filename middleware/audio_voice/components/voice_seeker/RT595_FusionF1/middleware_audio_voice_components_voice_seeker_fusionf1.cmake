# Add set(CONFIG_USE_middleware_audio_voice_components_voice_seeker_fusionf1 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
        )
    endif()

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DFUSIONDSP=1
              )
  
  
  endif()

      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/libvoiceseeker_no_aec.a
        -Wl,--end-group
    )
    endif()

  