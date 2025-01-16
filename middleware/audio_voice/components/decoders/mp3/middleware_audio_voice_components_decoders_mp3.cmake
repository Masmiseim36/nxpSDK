# Add set(CONFIG_USE_middleware_audio_voice_components_decoders_mp3 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL iar OR CONFIG_TOOLCHAIN STREQUAL mcux) AND (CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )
    endif()

  
      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/armgcc/release/libmp3.a
        -Wl,--end-group
    )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm33)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/armgcc/release/libmp3.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/libs/cm7f/iar/release/libmp3.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/libs/cm33f/iar/release/libmp3.a
        -Wl,--end-group
    )
    endif()

  