# Add set(CONFIG_USE_middleware_cadence_codecs_mp3_enc_rt500_fusionf1 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../include
          ${CMAKE_CURRENT_LIST_DIR}/include/mp3_enc
        )

  
      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT500_FusionF1/xa_mp3_enc.a
        -Wl,--end-group
    )
    endif()

  