# Add set(CONFIG_USE_middleware_dsp_audio_streamer_testxa_af_hostless true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/xa-factory.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/aac_dec/xa-aac-decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/mixer/xa-mixer.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_dec/xa-mp3-decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_enc/xa-mp3-encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/opus_header.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/xa-opus-decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_enc/xa-opus-encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/pcm_gain/xa-pcm-gain.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/src-pp/xa-src-pp.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/cadence/vorbis_dec/xa-vorbis-decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-dmic.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-i2s.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/client_proxy/client_proxy.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/pcm_stereo/xa-pcm-stereo.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/sbc_dec/xa-sbc-decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/sbc_enc/xa-sbc-encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/renderer/xa-renderer.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/include
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/include/audio
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/client_proxy
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/vit_pre_proc
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/plugins/nxp/voice_seeker
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/testxa_af_hostless/test/test_inp
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DFIO_LOCAL_FS
  )

endif()

