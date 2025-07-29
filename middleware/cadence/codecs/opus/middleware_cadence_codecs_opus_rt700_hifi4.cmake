# Add set(CONFIG_USE_middleware_cadence_codecs_opus_rt700_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../include
          ${CMAKE_CURRENT_LIST_DIR}/include/opus_codec
          ${CMAKE_CURRENT_LIST_DIR}/include/ogg_lib
        )

  
      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/xa_opus_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/xa_opus_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/xa_opus_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/silk/xa_opus_silk_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/silk/xa_opus_silk_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/silk/xa_opus_silk_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/celt/xa_opus_celt_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/celt/xa_opus_celt_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/lib/RT700_HiFi4/celt/xa_opus_celt_enc.a
        -Wl,--end-group
    )
    endif()

  