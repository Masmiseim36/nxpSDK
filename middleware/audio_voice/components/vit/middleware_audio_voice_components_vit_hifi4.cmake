# Add set(CONFIG_USE_middleware_audio_voice_components_vit_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/HIFI4/Lib
          ${CMAKE_CURRENT_LIST_DIR}/HIFI4/Lib/Inc
        )
    endif()

  
      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/HIFI4/Lib/libVIT_HIFI4_v04_11_00.a
        -Wl,--end-group
    )
    endif()

  