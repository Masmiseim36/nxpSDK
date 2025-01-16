# Add set(CONFIG_USE_middleware_cadence_codecs_hifi4_rt700 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lc3/hifi4/lib/xa_lc3_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/mp3_enc/hifi4/lib/xa_mp3_enc.a
        -Wl,--end-group
    )
    endif()

  