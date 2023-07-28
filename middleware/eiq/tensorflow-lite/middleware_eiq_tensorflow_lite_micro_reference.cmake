# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_reference true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_tensorflow_lite_micro)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/activations.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/add.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/depthwise_conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/fully_connected.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/leaky_relu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/logistic.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/lstm_eval.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/mul.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/pad.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/pooling.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/quantize.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/reduce.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/reshape.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/svdf.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/softmax.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/strided_slice.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/sub.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/transpose_conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/unidirectional_sequence_lstm.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro_reference dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
