include_guard(GLOBAL)
message("middleware_eiq_tensorflow_lite_micro component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/c/common.c
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/error_reporter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/flatbuffer_conversions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/op_resolver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/tensor_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/kernels/kernel_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/kernels/internal/quantization_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/all_ops_resolver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/debug_log.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/memory_helpers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_allocator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_error_reporter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_graph.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_interpreter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_profiler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_string.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/recording_micro_allocator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/recording_simple_memory_allocator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/simple_memory_allocator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/add_n.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/arg_min_max.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/batch_to_space_nd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cast.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/ceil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/circular_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/comparisons.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/concatenation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/conv_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cumsum.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/depth_to_space.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/depthwise_conv_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/dequantize.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/detection_postprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/elementwise.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/elu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/ethosu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/exp.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/expand_dims.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/fill.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor_div.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor_mod.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/fully_connected_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/gather.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/gather_nd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/hard_swish.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/if.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/kernel_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/l2_pool_2d.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/l2norm.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/leaky_relu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/log_softmax.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/logical.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/maximum_minimum.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/neg.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/pack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/pad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/prelu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/quantize_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/reduce.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/reshape.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/resize_bilinear.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/resize_nearest_neighbor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/round.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/shape.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/softmax_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/space_to_batch_nd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/split.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/split_v.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/squeeze.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/strided_slice.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/sub.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/svdf_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/tanh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/transpose.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/transpose_conv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/unpack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/zeros_like.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/memory_planner/linear_memory_planner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/schema/schema_utils.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1176_cm4)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1176_cm4)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1176_cm4)

