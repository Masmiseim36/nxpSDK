# Add set(CONFIG_USE_middleware_dsp_audio_streamer_hifi4_dec true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt685 OR CONFIG_BOARD STREQUAL mimxrt685audevk))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/codecs/aacplus_dec/hifi4/include/aac_dec
  ${CMAKE_CURRENT_LIST_DIR}/codecs/asrc_src_pp/hifi4/include/src_pp
  ${CMAKE_CURRENT_LIST_DIR}/codecs/g711/hifi4/include/g711_codec
  ${CMAKE_CURRENT_LIST_DIR}/codecs/mp3_dec/hifi4/include/mp3_dec
  ${CMAKE_CURRENT_LIST_DIR}/codecs/opus/hifi4/include/opus_codec
  ${CMAKE_CURRENT_LIST_DIR}/codecs/opus/hifi4/include/ogg_lib
  ${CMAKE_CURRENT_LIST_DIR}/codecs/sbc_dec/hifi4/include/sbc_dec
  ${CMAKE_CURRENT_LIST_DIR}/codecs/sbc_enc/hifi4/include/sbc_enc
  ${CMAKE_CURRENT_LIST_DIR}/codecs/vorbis_dec/hifi4/include/vorbis_dec
)

if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/codecs/aacplus_dec/hifi4/lib/xa_aacplus_v2_loas_dec.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/asrc_src_pp/hifi4/lib/xa_asrc_src_pp.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/g711/hifi4/lib/xa_g711_codec.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/mp3_dec/hifi4/lib/xa_mp3_dec.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/opus/hifi4/lib/xa_opus_codec.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/opus/hifi4/lib/xa_opus_dec.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/opus/hifi4/lib/xa_opus_enc.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/sbc_dec/hifi4/lib/xa_sbc_dec.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/sbc_enc/hifi4/lib/xa_sbc_enc.a
      ${CMAKE_CURRENT_LIST_DIR}/codecs/vorbis_dec/hifi4/lib/xa_vorbis_dec.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_dsp_audio_streamer_hifi4_dec dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
