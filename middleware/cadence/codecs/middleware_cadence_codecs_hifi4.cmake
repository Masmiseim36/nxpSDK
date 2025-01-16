# Add set(CONFIG_USE_middleware_cadence_codecs_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/aacplus_dec/hifi4/include/aac_dec
          ${CMAKE_CURRENT_LIST_DIR}/asrc_src_pp/hifi4/include/src_pp
          ${CMAKE_CURRENT_LIST_DIR}/bsac/hifi4/include/aac_dec
          ${CMAKE_CURRENT_LIST_DIR}/dabplus/hifi4/include/aac_dec
          ${CMAKE_CURRENT_LIST_DIR}/drm/hifi4/include/drm_dec
          ${CMAKE_CURRENT_LIST_DIR}/g711/hifi4/include/g711_codec
          ${CMAKE_CURRENT_LIST_DIR}/g722/hifi4/include/g722_codec
          ${CMAKE_CURRENT_LIST_DIR}/mp3_dec/hifi4/include/mp3_dec
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/include/opus_codec
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/include/ogg_lib
          ${CMAKE_CURRENT_LIST_DIR}/sbc_dec/hifi4/include/sbc_dec
          ${CMAKE_CURRENT_LIST_DIR}/sbc_enc/hifi4/include/sbc_enc
          ${CMAKE_CURRENT_LIST_DIR}/vorbis_dec/hifi4/include/vorbis_dec
        )

  
      if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/mp2/hifi4/lib/xa_mp2_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/silk/xa_opus_silk_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/silk/xa_opus_silk_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/silk/xa_opus_silk_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/celt/xa_opus_celt_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/celt/xa_opus_celt_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/celt/xa_opus_celt_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/aacplus_dec/hifi4/lib/xa_aacplus_v2_loas_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/asrc_src_pp/hifi4/lib/xa_asrc_src_pp.a
          ${CMAKE_CURRENT_LIST_DIR}/bsac/hifi4/lib/xa_bsac_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/dabplus/hifi4/lib/xa_dabplus_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/drm/hifi4/lib/xa_drm_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/g711/hifi4/lib/xa_g711_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/g722/hifi4/lib/xa_g722_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/mp3_dec/hifi4/lib/xa_mp3_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/xa_opus_codec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/xa_opus_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/opus/hifi4/lib/xa_opus_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/sbc_dec/hifi4/lib/xa_sbc_dec.a
          ${CMAKE_CURRENT_LIST_DIR}/sbc_enc/hifi4/lib/xa_sbc_enc.a
          ${CMAKE_CURRENT_LIST_DIR}/vorbis_dec/hifi4/lib/xa_vorbis_dec.a
        -Wl,--end-group
    )
    endif()

  