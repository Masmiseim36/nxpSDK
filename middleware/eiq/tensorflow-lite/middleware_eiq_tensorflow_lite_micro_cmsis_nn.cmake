# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_cmsis_nn true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_tensorflow_lite_micro AND (CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm33 OR CONFIG_CORE STREQUAL cm7f) AND CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/add.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/mul.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/pooling.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/softmax.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/svdf.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/activations.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/ethosu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/leaky_relu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/logistic.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/lstm_eval.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/pad.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/quantize.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/reduce.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/reshape.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/strided_slice.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/sub.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/transpose_conv.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCMSIS_NN
  )

endif()

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro_cmsis_nn dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
