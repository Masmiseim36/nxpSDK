# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_xtensa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_tensorflow_lite_micro AND ((CONFIG_DEVICE_ID STREQUAL MIMXRT595S OR CONFIG_DEVICE_ID STREQUAL MIMXRT685S) OR (CONFIG_CORE STREQUAL dsp)) AND CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4_binary)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/add.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/add_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/conv_hifi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/conv_int16_reference.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/conv_int8_reference.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/conv_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/depthwise_conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/depthwise_conv_hifi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/depthwise_conv_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/fully_connected.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/fully_connected_common_xtensa.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/fully_connected_int8.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/fully_connected_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/leaky_relu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/logistic.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/lstm_eval.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/lstm_eval_hifi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/pad.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/pad_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/pooling.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/pooling_int8.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/pooling_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/quantize.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/reduce.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/reduce_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/reshape.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/reshape_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/softmax.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/softmax_int8_int16.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/softmax_vision.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/strided_slice.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/sub.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/svdf.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/transpose_conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/xtensa/unidirectional_sequence_lstm.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/activations.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/ethosu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/mul.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DXTENSA
    -DHIFI4
  )

endif()

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro_xtensa dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
