include_guard(GLOBAL)
message("middleware_eiq_tensorflow_lite_micro_cmsis_nn component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/add.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/conv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/mul.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/pooling.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/softmax.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/svdf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/activations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/floor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/logistic.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite
)


include(middleware_eiq_tensorflow_lite_micro_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT1176_cm7)

include(CMSIS_DSP_Library_MIMXRT1176_cm7)

