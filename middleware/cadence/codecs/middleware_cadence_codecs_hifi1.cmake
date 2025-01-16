# Add set(CONFIG_USE_middleware_cadence_codecs_hifi1 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/aacplus_dec/hifi1/include/aac_dec
          ${CMAKE_CURRENT_LIST_DIR}/asrc_src_pp/hifi1/include/src_pp
          ${CMAKE_CURRENT_LIST_DIR}/mp3_dec/hifi1/include/mp3_dec
          ${CMAKE_CURRENT_LIST_DIR}/mp3_enc/hifi1/include/mp3_enc
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/include/opus_codec
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/include/ogg_lib
          ${CMAKE_CURRENT_LIST_DIR}/sbc_dec/hifi1/include/sbc_dec
          ${CMAKE_CURRENT_LIST_DIR}/sbc_enc/hifi1/include/sbc_enc
          ${CMAKE_CURRENT_LIST_DIR}/vorbis_dec/hifi1/include/vorbis_dec
        )

  
      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/silk/xa_opus_silk_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/silk/xa_opus_silk_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/silk/xa_opus_silk_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/celt/xa_opus_celt_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/celt/xa_opus_celt_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/celt/xa_opus_celt_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/aacplus_dec/hifi1/lib/xa_aacplus_v2_loas_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/asrc_src_pp/hifi1/lib/xa_asrc_src_pp.a
          ${CMAKE_CURRENT_LIST_DIR}/lc3/hifi1/lib/xa_lc3_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/lc3/hifi1/lib/xa_lc3_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/lc3/hifi1/lib/xa_lc3_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/mp3_dec/hifi1/lib/xa_mp3_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/mp3_enc/hifi1/lib/xa_mp3_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/xa_opus_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/xa_opus_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi1/lib/xa_opus_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/sbc_dec/hifi1/lib/xa_sbc_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/sbc_enc/hifi1/lib/xa_sbc_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/vorbis_dec/hifi1/lib/xa_vorbis_dec.a
        -Wl,--end-group
    )
    endif()

  