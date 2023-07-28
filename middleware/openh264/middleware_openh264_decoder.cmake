# Add set(CONFIG_USE_middleware_openh264_decoder true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_openh264_api AND CONFIG_USE_middleware_openh264_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/au_parser.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/bit_stream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/cabac_decoder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/deblocking.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/decode_mb_aux.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/decode_slice.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/decoder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/decoder_core.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/decoder_data_tables.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/error_concealment.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/fmo.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/get_intra_predictor.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/manage_dec_ref.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/memmgr_nal_unit.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/mv_pred.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/parse_mb_syn_cabac.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/parse_mb_syn_cavlc.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/pic_queue.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/rec_mb.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/src/wels_decoder_thread.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/plus/src/welsDecoderExt.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/core/inc
  ${CMAKE_CURRENT_LIST_DIR}/codec/decoder/plus/inc
)

else()

message(SEND_ERROR "middleware_openh264_decoder dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
