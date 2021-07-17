INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXTENSA_NNLIB_MAX_SCRATCH_SIZE=258*1024")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNNLIB_V2")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMODEL_INT16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Dhifi4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-div32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mlongcalls")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -INLINE:requested")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcoproc")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-zero-initialized-in-bss")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mtext-section-literals")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-unsafe-math-optimizations")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=nxp_rt500_RI2020_5_newlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXTENSA_NNLIB_MAX_SCRATCH_SIZE=258*1024")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNNLIB_V2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMODEL_INT16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Dhifi4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-div32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mlongcalls")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -INLINE:requested")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcoproc")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-zero-initialized-in-bss")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mtext-section-literals")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-unsafe-math-optimizations")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=nxp_rt500_RI2020_5_newlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DXTENSA_NNLIB_MAX_SCRATCH_SIZE=258*1024")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNNLIB_V2")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMODEL_INT16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Dhifi4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --stdlib=libc++")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mlongcalls")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-variable")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DXTENSA_NNLIB_MAX_SCRATCH_SIZE=258*1024")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNNLIB_V2")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMODEL_INT16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Dhifi4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --stdlib=libc++")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mlongcalls")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mlsp=${ProjDirPath}/../gdbio")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-core=nxp_rt500_RI2020_5_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mlsp=${ProjDirPath}/../min-rt")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-core=nxp_rt500_RI2020_5_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/label_image)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/common)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/common/image)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/common/model)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/label_image/ptq)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Core/Include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/ruy)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/nnlib)

add_executable(tensorflow_lite_micro_label_image_fusionf1.elf 
"${ProjDirPath}/../../../../../../middleware/eiq/common/demo_info.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/image/image.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/image/image_decode_raw.c"
"${ProjDirPath}/../../../../../../middleware/eiq/common/image/image_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/model/get_top_n.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/common/model/get_top_n.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/model/model.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/model/output_postproc.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/common/model/output_postproc.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/label_image/demo_config.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/label_image/image_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/label_image/labels.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/label_image/model_mobilenet_ops_micro.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/model.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/image/main.cpp"
"${ProjDirPath}/../core_dsp.h"
"${ProjDirPath}/../gdbio/ldscripts/elf32xtensa.x"
"${ProjDirPath}/../gdbio/specs"
"${ProjDirPath}/../min-rt/ldscripts/elf32xtensa.x"
"${ProjDirPath}/../min-rt/specs"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/examples/label_image/ptq/model_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/image/image_load.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_fusionf1.c"
"${ProjDirPath}/../board_fusionf1.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/timer.h"
"${ProjDirPath}/../../../../../../middleware/eiq/common/timer_xtensa.c"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_uart.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite/fsl_assert.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/core/public/version.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/portable_type_to_tflitetype.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/version.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/common.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/builtin_op_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/error_reporter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/flatbuffer_conversions.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/op_resolver.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/profiler.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/tensor_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/error_reporter.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/flatbuffer_conversions.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/op_resolver.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/tensor_utils.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/kernel_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/kernel_utils.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/op_macros.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/padding.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/compatibility.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/cppmath.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/max.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/min.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/portable_tensor.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/quantization_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/quantization_util.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/strided_slice_logic.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/tensor_ctypes.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/types.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/optimized/neon_check.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/add.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/arg_min_max.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/binary_function.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/ceil.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/comparisons.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/concatenation.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/depthwiseconv_float.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/depthwiseconv_uint8.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/dequantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/floor.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/fully_connected.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/hard_swish.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/l2normalization.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/logistic.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/maximum_minimum.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/mul.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/neg.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/pad.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/pooling.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/prelu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/process_broadcast_shapes.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/quantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/reduce.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/requantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/resize_nearest_neighbor.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/round.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/softmax.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/strided_slice.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/sub.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/tanh.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/add.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/depthwise_conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/fully_connected.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/l2normalization.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/logistic.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/mean.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/mul.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/pooling.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/tanh.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/all_ops_resolver.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/all_ops_resolver.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/compatibility.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_helpers.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_helpers.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_allocator.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_error_reporter.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_error_reporter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_interpreter.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_interpreter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_mutable_op_resolver.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_op_resolver.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_profiler.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_profiler.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_string.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_string.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_time.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_time.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_utils.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/recording_micro_allocator.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/recording_micro_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/recording_micro_interpreter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/recording_simple_memory_allocator.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/recording_simple_memory_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/simple_memory_allocator.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/simple_memory_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/activation_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/fully_connected.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/kernel_runner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/kernel_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/micro_ops.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/micro_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/arg_min_max.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/ceil.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/circular_buffer.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/comparisons.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/concatenation.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/dequantize.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/elementwise.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/ethosu.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/hard_swish.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/kernel_runner.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/kernel_util.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/l2norm.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/logical.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/maximum_minimum.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/neg.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/pack.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/pad.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/prelu.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/quantize.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/reduce.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/reshape.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/resize_nearest_neighbor.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/round.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/shape.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/split.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/split_v.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/strided_slice.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/sub.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/tanh.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/unpack.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/greedy_memory_planner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/linear_memory_planner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/memory_planner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/linear_memory_planner.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/schema/schema_utils.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/schema/schema_generated.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/schema/schema_utils.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/MIMXRT595S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/MIMXRT595S_dsp_features.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/system_MIMXRT595S_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/system_MIMXRT595S_dsp.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/activations.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/add.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/conv.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/depthwise_conv.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/floor.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/fully_connected.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/logistic.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/mul.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/pooling.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/softmax.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/svdf.cpp"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa_hifi/xtensa_tf_micro_common.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../CMSIS/Core/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Core/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../CMSIS/Core/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../../CMSIS/Core/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../CMSIS/Core/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Core/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../CMSIS/Core/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/base.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flatbuffers.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/stl_emulation.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp/fixedpoint/fixedpoint.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp/internal/detect_platform.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/ruy/ruy/profiler/instrumentation.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/include/xa_api_defs.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/include/xa_nnlib_common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/include/xa_nnlib_common_macros.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/include/xa_nnlib_definitions.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/include/xa_nnlib_err_chk.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/include/xa_nnlib_hifi_isa_compat.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/common/src/xa_nnlib_common_api.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_activations_16_16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_activations_32_16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_activations_32_32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_activations_32_8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_activations_8_8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_activations_asym8_asym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_activations_f32_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/activations/hifi4/xa_nn_softmax_asym8_asym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_add_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_add_quant8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_div_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_mul_16x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_mul_acc_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_mul_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_mul_quant8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_elm_sub_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_floor_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_vec_interpolation_q15.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/basic/hifi4/xa_nn_basic_state.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_circ_buf.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_16x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_8x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_8x8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_asym8xasym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_circ_buf.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_16x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_8x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_8x8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_asym8xasym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_16x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_8x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_8x8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_asym8xasym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_16x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_8x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_8x8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_asym8xasym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_circ_buf.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_16x16_16_circ.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_16x16_16_circ_nb.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x16_16_circ.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x16_16_circ_nb.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x8_8_circ.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x8_8_circ_nb.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_asym8xasym8_asym8_circ.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_asym8xasym8_asym8_circ_nb.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_f32_circ.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_matXvec_f32_circ_nb.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_circ_buf.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_state.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_state.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_state.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/fc/hifi4/xa_nn_fully_connected.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_16x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_16x16_batch.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_16x8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x16_batch.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x8_batch.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_asym8xasym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_asym8xasym8_batch.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matXvec_f32_batch.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matmul_16x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matmul_8x16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matmul_8x8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matmul_asym8xasym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/matXvec/hifi4/xa_nn_matmul_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/norm/hifi4/xa_nn_l2_norm_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_16_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_8_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_asym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_asym8_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_f32_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_inv_256_tbl.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_16.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_16_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_8_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_asym8.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_asym8_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_f32.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_f32_nhwc.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_avgpool_state.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/kernels/pool/hifi4/xa_nn_maxpool_state.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/layers/cnn/src/xa_nn_cnn_api.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/layers/gru/src/xa_nn_gru_api.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/layers/lstm/src/xa_nn_lstm_api.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/NatureDSP_Signal_math.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/NatureDSP_Signal_matop.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/NatureDSP_types.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/common_fpu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/expf_tbl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/inff_tbl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/nan_tbl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/nanf_tbl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/pow2f_tbl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/include/tanhf_tbl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/expf_tbl.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/inff_tbl.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/nanf_tbl.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/pow2f_tbl.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/scl_sigmoidf_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/scl_tanhf_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/tanhf_tbl.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_alognf_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_relu32x32_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_reluf_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_sigmoid32x32_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_sigmoidf_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_softmax32x32_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_softmaxf_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_tanh32x32_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/algo/ndsp/hifi4/src/vec_tanhf_hifi4.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/xa_type_def.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/nnlib/xa_nnlib_api.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/nnlib/xa_nnlib_cnn_api.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/nnlib/xa_nnlib_gru_api.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/nnlib/xa_nnlib_kernels_api.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/nnlib/xa_nnlib_lstm_api.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/xa_nnlib_hifi4/include/nnlib/xa_nnlib_standards.h"
)



ADD_CUSTOM_COMMAND(TARGET tensorflow_lite_micro_label_image_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/tensorflow_lite_micro_label_image_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2020_5_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.ResetVector.text
--only-section=.WindowVectors.text
--only-section=.Level2InterruptVector.text
--only-section=.Level3InterruptVector.text
--only-section=.DebugExceptionVector.text
--only-section=.NMIExceptionVector.text
--only-section=.KernelExceptionVector.text
--only-section=.UserExceptionVector.literal
--only-section=.UserExceptionVector.text
--only-section=.DoubleExceptionVector.text
--only-section=.text
)

ADD_CUSTOM_COMMAND(TARGET tensorflow_lite_micro_label_image_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/tensorflow_lite_micro_label_image_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2020_5_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.rodata
--only-section=.clib.data
--only-section=.rtos.percpu.data
--only-section=.data
--only-section=.bss
)

