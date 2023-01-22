include_guard()
message("middleware_maestro_framework_streamer component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/core/pad.c
    ${CMAKE_CURRENT_LIST_DIR}/core/pipeline.c
    ${CMAKE_CURRENT_LIST_DIR}/core/streamer.c
    ${CMAKE_CURRENT_LIST_DIR}/core/streamer_element.c
    ${CMAKE_CURRENT_LIST_DIR}/core/streamer_msg.c
    ${CMAKE_CURRENT_LIST_DIR}/decoders/cci_dec/ccidec.c
    ${CMAKE_CURRENT_LIST_DIR}/decoders/cci_dec/codec_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/decoders/cci_dec/codecextractmetadata.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/audio_sink_pcmrtos.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/audio_src_pcmrtos.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/audio_proc.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/audio_sink.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/audio_src.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/decoder.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/decoder_pads.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/file_sink.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/file_src_freertos.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/mem_sink.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/vit_sink.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/mem_src.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/netbuf_src.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/cci/cci_codec_type_conversion.c
    ${CMAKE_CURRENT_LIST_DIR}/pipelines/streamer_audiosrc.c
    ${CMAKE_CURRENT_LIST_DIR}/pipelines/streamer_fs.c
    ${CMAKE_CURRENT_LIST_DIR}/pipelines/streamer_mic2file.c
    ${CMAKE_CURRENT_LIST_DIR}/pipelines/streamer_opusmem2mem.c
    ${CMAKE_CURRENT_LIST_DIR}/pipelines/streamer_pcm_speaker.c
    ${CMAKE_CURRENT_LIST_DIR}/pipelines/streamer_testeapfile2file.c
    ${CMAKE_CURRENT_LIST_DIR}/pipelines/streamer_vit.c
    ${CMAKE_CURRENT_LIST_DIR}/elements/encoder.c
    ${CMAKE_CURRENT_LIST_DIR}/encoders/opus/opusenc_cei.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/mp3/mp3_extractmetadata.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/mp3/mp3_id3v2.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/mp3/mp3_vbr.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/adpcm/adpcm_extractmetadata.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/adpcm/wav_common.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/opus/opus_extractmetadata.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/aac/aac_extractmetadata.c
    ${CMAKE_CURRENT_LIST_DIR}/cci/metadata/src/flac/flac_extractmetadata.c
    ${CMAKE_CURRENT_LIST_DIR}/utils/file_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/utils/general_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/utils/maestro_logging.c
    ${CMAKE_CURRENT_LIST_DIR}/utils/ringbuffer.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cci/include
    ${CMAKE_CURRENT_LIST_DIR}/core
    ${CMAKE_CURRENT_LIST_DIR}/decoders/cci_dec
    ${CMAKE_CURRENT_LIST_DIR}/devices
    ${CMAKE_CURRENT_LIST_DIR}/elements
    ${CMAKE_CURRENT_LIST_DIR}/encoders/opus
    ${CMAKE_CURRENT_LIST_DIR}/encoders/cei
    ${CMAKE_CURRENT_LIST_DIR}/pipelines
)


