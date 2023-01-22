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

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(glow_cifar10_fusionf1)

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXOS_DEBUG_ALL=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXOS_CLOCK_FREQ=198000000")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_CORE_ID=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_CFG_CORES_NUM=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNN_ENABLE_xa_nn_inference=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNNLIB_V2")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -INLINE:requested")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-div32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mlongcalls")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fpermissive")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXOS_DEBUG_ALL=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXOS_CLOCK_FREQ=198000000")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXF_CORE_ID=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXF_CFG_CORES_NUM=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNN_ENABLE_xa_nn_inference=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNNLIB_V2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -INLINE:requested")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-div32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mlongcalls")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fpermissive")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNNLIB_V2")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNNLIB_V2")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT595S/xtensa/gdbio")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT595S/xtensa/min-rt")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

include_directories(${ProjDirPath}/../glow_bundle)

include_directories(${ProjDirPath}/../glow_bundle)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../glow_bundle)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt500_fusionf1)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/environment/bm)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Core/Include)

add_executable(glow_cifar10_fusionf1.elf 
"${ProjDirPath}/../board_fusionf1.c"
"${ProjDirPath}/../board_fusionf1.h"
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../srtm_config.h"
"${ProjDirPath}/../srtm_config_audio.h"
"${ProjDirPath}/../srtm_config_nn.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../main_hifi4.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../dsp_nn.c"
"${ProjDirPath}/../dsp_nn.h"
"${ProjDirPath}/../config_nn.h"
"${ProjDirPath}/../dsp_resizenearest.c"
"${ProjDirPath}/../dsp_resizenearest.h"
"${ProjDirPath}/../glow_bundle/model.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2c_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2c_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2s.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2s.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2s_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_i2s_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_sema42.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_sema42.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_spi_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_spi_dma.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_uart.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_ctimer.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_ctimer.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt500_fusionf1/rpmsg_platform.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imxrt500_fusionf1/rpmsg_platform.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_memory.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_pint.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_pint.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dmic.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dmic.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dmic_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dmic_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_common_dsp.h"
"${ProjDirPath}/../../../../../../components/lists/fsl_component_generic_list.h"
"${ProjDirPath}/../../../../../../components/lists/fsl_component_generic_list.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_iopctl.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_ns.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_bm.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/environment/bm/rpmsg_env_specific.h"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/LICENSE"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc/xa_nnlib_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc/xa_nnlib_cnn_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc/xa_nnlib_gru_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc/xa_nnlib_kernels_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc/xa_nnlib_lstm_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc/xa_nnlib_standards.h"
"${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc/xa_type_def.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/MIMXRT595S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/MIMXRT595S_dsp_features.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/system_MIMXRT595S_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/system_MIMXRT595S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_flexcomm.c"
)


TARGET_LINK_LIBRARIES(glow_cifar10_fusionf1.elf -Wl,--start-group)
target_link_libraries(glow_cifar10_fusionf1.elf debug xos)

target_link_libraries(glow_cifar10_fusionf1.elf debug xtutil)

target_link_libraries(glow_cifar10_fusionf1.elf debug m)

target_link_libraries(glow_cifar10_fusionf1.elf optimized xos)

target_link_libraries(glow_cifar10_fusionf1.elf optimized xtutil)

target_link_libraries(glow_cifar10_fusionf1.elf optimized m)


target_link_libraries(glow_cifar10_fusionf1.elf debug ${ProjDirPath}/../glow_bundle/model.o)


target_link_libraries(glow_cifar10_fusionf1.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/lib/fusionf1/libxa_nnlib.a)


target_link_libraries(glow_cifar10_fusionf1.elf optimized ${ProjDirPath}/../glow_bundle/model.o)


target_link_libraries(glow_cifar10_fusionf1.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/lib/fusionf1/libxa_nnlib.a)

TARGET_LINK_LIBRARIES(glow_cifar10_fusionf1.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET glow_cifar10_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/glow_cifar10_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_reset_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.ResetVector.text
)

ADD_CUSTOM_COMMAND(TARGET glow_cifar10_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/glow_cifar10_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.WindowVectors.text
--only-section=.Level2InterruptVector.text
--only-section=.Level3InterruptVector.text
--only-section=.DebugExceptionVector.text
--only-section=.NMIExceptionVector.text
--only-section=.KernelExceptionVector.text
--only-section=.UserExceptionVector.text
--only-section=.DoubleExceptionVector.text
--only-section=.text
)

ADD_CUSTOM_COMMAND(TARGET glow_cifar10_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/glow_cifar10_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.rtos.rodata
--only-section=.rodata
--only-section=.rtos.percpu.data
--only-section=.data
--only-section=.bss
)

