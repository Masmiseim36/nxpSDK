if(NOT MIDDLEWARE_EIQ_TENSORFLOW_LITE_MICRO_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_EIQ_TENSORFLOW_LITE_MICRO_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_eiq_tensorflow_lite_micro component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/c/common.c
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/core/api/op_resolver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/core/api/flatbuffer_conversions.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/core/api/tensor_utils_.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/core/api/error_reporter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/kernels/kernel_util.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/kernels/internal/quantization_util.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/recording_micro_allocator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/debug_log.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/memory_helpers.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_error_reporter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/all_ops_resolver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_string.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_profiler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_time.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_allocator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/simple_memory_allocator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_interpreter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/micro_optional_debug_tools.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/recording_simple_memory_allocator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/activations.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/arg_min_max.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/ceil.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/circular_buffer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/comparisons.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/concatenation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/dequantize.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/elementwise.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/ethosu.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/floor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/l2norm.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/logical.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/logistic.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/maximum_minimum.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/neg.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/pack.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/pad.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/prelu.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/quantize.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/reduce.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/reshape.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/resize_nearest_neighbor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/round.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/split.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/strided_slice.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/sub.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/svdf.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/tanh.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/unpack.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/conv.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/add.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/softmax.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/fully_connected.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/depthwise_conv.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/mul.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/kernels/cmsis-nn/pooling.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/memory_planner/linear_memory_planner.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/tensorflow/lite/string_util.cpp
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite
    )


    include(middleware_eiq_tensorflow_lite_third_party_cmsis_MIMXRT1052)

    include(middleware_eiq_tensorflow_lite_third_party_flatbuffers_MIMXRT1052)

    include(middleware_eiq_tensorflow_lite_third_party_gemmlowp_MIMXRT1052)

    include(middleware_eiq_tensorflow_lite_third_party_ruy_MIMXRT1052)

endif()
