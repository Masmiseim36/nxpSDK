# Add set(CONFIG_USE_middleware_maestro_framework true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_maestro_framework_doc AND CONFIG_USE_component_osa_free_rtos AND CONFIG_USE_middleware_fatfs AND CONFIG_USE_middleware_maestro_framework_template)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/streamer/core/pad.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/core/pipeline.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/core/streamer.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/core/streamer_element.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/core/streamer_msg.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/decoders/cci_dec/ccidec.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/decoders/cci_dec/codec_interface.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/decoders/cci_dec/codecextractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/devices/audio_sink_pcmrtos.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/devices/audio_src_pcmrtos.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/audio_proc.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/audio_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/audio_src.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/decoder_pads.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/file_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/file_src_freertos.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/mem_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/vit_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/mem_src.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/netbuf_src.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/parsers/cci/cci_codec_type_conversion.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements/encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/encoders/opus/opusenc_cei.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/mp3/mp3_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/mp3/mp3_id3v2.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/mp3/mp3_vbr.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/adpcm/adpcm_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/adpcm/wav_common.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/opus/opus_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/aac/aac_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/metadata/src/flac/flac_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/utils/file_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/utils/general_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/utils/maestro_logging.c
  ${CMAKE_CURRENT_LIST_DIR}/streamer/utils/ringbuffer.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/config
  ${CMAKE_CURRENT_LIST_DIR}/inc
  ${CMAKE_CURRENT_LIST_DIR}/streamer/cci/include
  ${CMAKE_CURRENT_LIST_DIR}/streamer/core
  ${CMAKE_CURRENT_LIST_DIR}/streamer/decoders/cci_dec
  ${CMAKE_CURRENT_LIST_DIR}/streamer/devices
  ${CMAKE_CURRENT_LIST_DIR}/streamer/elements
  ${CMAKE_CURRENT_LIST_DIR}/streamer/encoders/opus
  ${CMAKE_CURRENT_LIST_DIR}/streamer/encoders/cei
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCASCFG_PLATFORM_FREERTOS
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS
    -DFSL_OSA_TASK_ENABLE=1
  )

endif()

else()

message(SEND_ERROR "middleware_maestro_framework dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
