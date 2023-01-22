include_guard()
message("middleware_eiq_tensorflow_lite_micro_cmsis_nn component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/add.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/conv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/mul.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/pooling.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/softmax.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cmsis_nn/svdf.cpp
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
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/unidirectional_sequence_lstm.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(middleware_eiq_tensorflow_lite_micro_MIMXRT595S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT595S_cm33)

