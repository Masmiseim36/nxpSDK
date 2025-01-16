# Add set(CONFIG_USE_middleware_cadence_codecs_fusionf1 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/aacplus_dec/fusionf1/include/aac_dec
          ${CMAKE_CURRENT_LIST_DIR}/asrc_src_pp/fusionf1/include/src_pp
          ${CMAKE_CURRENT_LIST_DIR}/mp3_dec/fusionf1/include/mp3_dec
          ${CMAKE_CURRENT_LIST_DIR}/mp3_enc/fusionf1/include/mp3_enc
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/include/opus_codec
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/include/ogg_lib
          ${CMAKE_CURRENT_LIST_DIR}/sbc_dec/fusionf1/include/sbc_dec
          ${CMAKE_CURRENT_LIST_DIR}/sbc_enc/fusionf1/include/sbc_enc
          ${CMAKE_CURRENT_LIST_DIR}/vorbis_dec/fusionf1/include/vorbis_dec
        )

  
      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/lib/silk/xa_opus_silk_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/lib/silk/xa_opus_silk_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/lib/silk/xa_opus_silk_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/lib/celt/xa_opus_celt_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/lib/celt/xa_opus_celt_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/lib/celt/xa_opus_celt_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/aacplus_dec/fusionf1/lib/xa_aacplus_v2_loas_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/asrc_src_pp/fusionf1/lib/xa_asrc_src_pp.a
          ${CMAKE_CURRENT_LIST_DIR}/mp3_dec/fusionf1/lib/xa_mp3_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/mp3_enc/fusionf1/lib/xa_mp3_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/fusionf1/lib/xa_opus_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/sbc_dec/fusionf1/lib/xa_sbc_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/sbc_enc/fusionf1/lib/xa_sbc_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/vorbis_dec/fusionf1/lib/xa_vorbis_dec.a
        -Wl,--end-group
    )
    endif()

  