# Add set(CONFIG_USE_middleware_openh264_encoder true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_openh264_api AND CONFIG_USE_middleware_openh264_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/au_set.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/deblocking.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/decode_mb_aux.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/encode_mb_aux.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/encoder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/encoder_data_tables.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/encoder_ext.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/get_intra_predictor.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/md.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/mv_pred.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/nal_encap.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/paraset_strategy.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/picture_handle.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/ratectl.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/ref_list_mgr_svc.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/sample.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/set_mb_syn_cabac.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/set_mb_syn_cavlc.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/slice_multi_threading.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_base_layer_md.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_enc_slice_segment.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_encode_mb.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_encode_slice.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_mode_decision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_motion_estimate.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_set_mb_syn_cabac.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/svc_set_mb_syn_cavlc.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/wels_preprocess.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/wels_task_base.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/wels_task_encoder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/src/wels_task_management.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/plus/src/DllEntry.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/plus/src/welsEncoderExt.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/core/inc
  ${CMAKE_CURRENT_LIST_DIR}/codec/encoder/plus/inc
)

else()

message(SEND_ERROR "middleware_openh264_encoder dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
