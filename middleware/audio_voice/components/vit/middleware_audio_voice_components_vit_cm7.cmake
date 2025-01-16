# Add set(CONFIG_USE_middleware_audio_voice_components_vit_cm7 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/CortexM7/Lib
          ${CMAKE_CURRENT_LIST_DIR}/CortexM7/Lib/Inc
        )
    endif()

  
      if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/CortexM7/Lib/libVIT_CM7_v04_11_00.a
        -Wl,--end-group
    )
    endif()

  