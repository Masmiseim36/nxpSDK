include_guard(GLOBAL)
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
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/logistic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/quantize.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(middleware_eiq_tensorflow_lite_micro_MIMXRT1176_cm4)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT1176_cm4)

