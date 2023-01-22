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


project(dsp_xaf_record_fusionf1)

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_DEBUG=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXOS_CLOCK_FREQ=198000000")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_CORE_ID=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_CFG_CORES_NUM=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_PCM_GAIN=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_VIT_PRE_PROC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_VOICE_SEEKER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_RENDERER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_CAPTURER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_DISABLE_EVENT=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEVICE_ID=VIT_IMXRT500")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__RENAMING__")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHAVE_XOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHIFI_ONLY_XAF")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFIO_LOCAL_FS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_RX_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNNLIB_V2")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-div32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mlongcalls")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -INLINE:requested")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXOS_CLOCK_FREQ=198000000")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXF_CORE_ID=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXF_CFG_CORES_NUM=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_PCM_GAIN=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_VIT_PRE_PROC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_VOICE_SEEKER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_RENDERER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_CAPTURER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_DISABLE_EVENT=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEVICE_ID=VIT_IMXRT500")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__RENAMING__")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHAVE_XOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHIFI_ONLY_XAF")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFIO_LOCAL_FS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_RX_ENABLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNNLIB_V2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT595SFFOC_dsp")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-div32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mlongcalls")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -INLINE:requested")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNNLIB_V2")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNNLIB_V2")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT595S/xtensa/gdbio")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT595S/xtensa/min-rt")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-core=nxp_rt500_RI2021_8_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt500_fusionf1)

include_directories(${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib)

include_directories(${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/Inc)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/environment/xos)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/lib)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/client_proxy)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/vit_pre_proc)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/voice_seeker)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/test_inp)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities)

include_directories(${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/naturedsp/fusionf1/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/inc)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT595S)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Core/Include)

include_directories(${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/include)

include_directories(${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public)

IF(${CMAKE_BUILD_TYPE} STREQUAL debug)
add_executable(dsp_xaf_record_fusionf1.elf 
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../srtm_config.h"
"${ProjDirPath}/../srtm_config_audio.h"
"${ProjDirPath}/../srtm_config_nn.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../xaf_main_dsp.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_fusionf1.c"
"${ProjDirPath}/../board_fusionf1.h"
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
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_uart.h"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_internal.h"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_manager.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_ctimer.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_ctimer.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_memory.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt500_fusionf1/rpmsg_platform.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imxrt500_fusionf1/rpmsg_platform.c"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/VIT.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/VIT_Model_cn.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/VIT_Model_en.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/Inc/PL_memoryRegion.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/Inc/PL_platformTypes_HIFI4_FUSIONF1.h"
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
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console/fsl_debug_console_conf.h"
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
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_xos.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_queue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_queue.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/environment/xos/rpmsg_env_specific.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/rbtree.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-audio-codec.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-base.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-capturer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mimo-proc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mixer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-renderer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-core.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-io.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipc-if.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipi.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-isr.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-main.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem-ipc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msg.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq1.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-sched.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-component.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-core.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-flx-lock-if.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-io.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-mm.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-msg.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-plugin.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-probe.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-sched.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-timebase.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-trace.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/audio/xa-class-base.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/lib/rbtree.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-ipc-if.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem-ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-shmem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-sys.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss/xf-board.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xaf-api.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-proxy.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-trace.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-structs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-threads-priority.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-version.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proto.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_af_hostless_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_api_defs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-ap-dp-common.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-msgq-if.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xaf-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xf-debug.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-audio-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-capturer-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-mimo-proc-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-mixer-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-renderer-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_apicmd_standards.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_error_standards.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_memory_standards.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_type_def.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-isr.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-msgq.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-thread.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-timer.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/host_fio.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-app-threads-priority.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-clk-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-fio-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-utils-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec22-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec23-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-microspeech-frontend-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-microspeech-inference-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-mimo-mix-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-encoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-gain-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-split-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-stereo-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-person-detect-inference-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_amr_wb_dec_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/xa-gain-factor-event.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/xa-factory.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/aac_dec/xa-aac-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mixer/xa-mixer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_dec/xa-mp3-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_enc/xa-mp3-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/opus_header.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/xa-opus-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/opus_header.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_enc/xa-opus-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/pcm_gain/xa-pcm-gain.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/src-pp/xa-src-pp.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/vorbis_dec/xa-vorbis-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-dmic.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-i2s.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/client_proxy/client_proxy.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/client_proxy/client_proxy_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/pcm_stereo/xa-pcm-stereo.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/sbc_dec/xa-sbc-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/sbc_enc/xa-sbc-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/renderer/xa-renderer.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/fsl_shell.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/fsl_shell.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/RdspCycleCounter.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/RdspDeviceConfig.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/RdspStatusCodes.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/libVoiceSeekerLight.h"
"${ProjDirPath}/../../../../../../middleware/dsp/naturedsp/fusionf1/include/NatureDSP_Signal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/naturedsp/fusionf1/include/NatureDSP_types.h"
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
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/include/RdspPlatforms.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/include/RdspTypes.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/RdspMemoryUtilsPublic.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/memcheck.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/RdspMemoryUtilsPublic.c"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/memcheck.c"
"${ProjDirPath}/../srtm_capturer_pcm_gain_renderer.c"
"${ProjDirPath}/../srtm_utils.c"
"${ProjDirPath}/../srtm_utils.h"
)
ENDIF()

IF(${CMAKE_BUILD_TYPE} STREQUAL release)
add_executable(dsp_xaf_record_fusionf1.elf 
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../srtm_config.h"
"${ProjDirPath}/../srtm_config_audio.h"
"${ProjDirPath}/../srtm_config_nn.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../xaf_main_dsp.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_fusionf1.c"
"${ProjDirPath}/../board_fusionf1.h"
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
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_uart.h"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_internal.h"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_manager.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_ctimer.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_ctimer.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers/fsl_memory.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt500_fusionf1/rpmsg_platform.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imxrt500_fusionf1/rpmsg_platform.c"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/VIT.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/VIT_Model_cn.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/VIT_Model_en.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/Inc/PL_memoryRegion.h"
"${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/Inc/PL_platformTypes_HIFI4_FUSIONF1.h"
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
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console/fsl_debug_console_conf.h"
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
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_xos.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_queue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_queue.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/environment/xos/rpmsg_env_specific.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/rbtree.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-audio-codec.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-base.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-capturer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mimo-proc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mixer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-renderer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-core.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-io.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipc-if.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipi.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-isr.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-main.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem-ipc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msg.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq1.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-sched.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-component.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-core.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-flx-lock-if.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-io.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-mm.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-msg.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-plugin.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-probe.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-sched.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-timebase.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-trace.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/audio/xa-class-base.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/lib/rbtree.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-ipc-if.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem-ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-shmem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-sys.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss/xf-board.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xaf-api.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-proxy.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-trace.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-structs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-threads-priority.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-version.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proto.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_af_hostless_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_api_defs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-ap-dp-common.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-msgq-if.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xaf-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xf-debug.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-audio-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-capturer-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-mimo-proc-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-mixer-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa-renderer-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_apicmd_standards.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_error_standards.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_memory_standards.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio/xa_type_def.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-isr.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-msgq.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-thread.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include/osal-timer.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/host_fio.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-app-threads-priority.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-clk-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-fio-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-utils-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec22-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec23-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-microspeech-frontend-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-microspeech-inference-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-mimo-mix-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-encoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-gain-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-split-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-stereo-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-person-detect-inference-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_amr_wb_dec_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/xa-gain-factor-event.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/xa-factory.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/aac_dec/xa-aac-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mixer/xa-mixer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_dec/xa-mp3-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_enc/xa-mp3-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/opus_header.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/xa-opus-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_dec/opus_header.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus_enc/xa-opus-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/pcm_gain/xa-pcm-gain.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/src-pp/xa-src-pp.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/vorbis_dec/xa-vorbis-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-dmic.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-i2s.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/client_proxy/client_proxy.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/client_proxy/client_proxy_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/pcm_stereo/xa-pcm-stereo.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/sbc_dec/xa-sbc-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/sbc_enc/xa-sbc-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/renderer/xa-renderer.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/fsl_shell.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/fsl_shell.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/RdspCycleCounter.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/RdspDeviceConfig.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/RdspStatusCodes.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/include/libVoiceSeekerLight.h"
"${ProjDirPath}/../../../../../../middleware/dsp/naturedsp/fusionf1/include/NatureDSP_Signal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/naturedsp/fusionf1/include/NatureDSP_types.h"
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
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/include/RdspPlatforms.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/include/RdspTypes.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/RdspMemoryUtilsPublic.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/memcheck.h"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/RdspMemoryUtilsPublic.c"
"${ProjDirPath}/../../../../../../middleware/voice_seeker/rdsp_utilities_public/rdsp_memory_utils_public/memcheck.c"
"${ProjDirPath}/../srtm_capturer_pcm_gain_renderer.c"
"${ProjDirPath}/../srtm_utils.c"
"${ProjDirPath}/../srtm_utils.h"
)
ENDIF()


TARGET_LINK_LIBRARIES(dsp_xaf_record_fusionf1.elf -Wl,--start-group)
target_link_libraries(dsp_xaf_record_fusionf1.elf debug xtutil)

target_link_libraries(dsp_xaf_record_fusionf1.elf debug m)

target_link_libraries(dsp_xaf_record_fusionf1.elf debug xos)

target_link_libraries(dsp_xaf_record_fusionf1.elf optimized m)

target_link_libraries(dsp_xaf_record_fusionf1.elf optimized xos)

target_link_libraries(dsp_xaf_record_fusionf1.elf optimized xtutil)


target_link_libraries(dsp_xaf_record_fusionf1.elf debug ${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/libVIT_Fusion_F1_v04_07_05.a)


target_link_libraries(dsp_xaf_record_fusionf1.elf debug ${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/lib/libvoiceseeker_no_aec.a)


target_link_libraries(dsp_xaf_record_fusionf1.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/naturedsp/fusionf1/libnaturedsp.a)


target_link_libraries(dsp_xaf_record_fusionf1.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/lib/fusionf1/libxa_nnlib.a)


target_link_libraries(dsp_xaf_record_fusionf1.elf optimized ${ProjDirPath}/../../../../../../middleware/vit/FusionF1/Lib/libVIT_Fusion_F1_v04_07_05.a)


target_link_libraries(dsp_xaf_record_fusionf1.elf optimized ${ProjDirPath}/../../../../../../middleware/voice_seeker/RT595_FusionF1/lib/libvoiceseeker_no_aec.a)


target_link_libraries(dsp_xaf_record_fusionf1.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/naturedsp/fusionf1/libnaturedsp.a)


target_link_libraries(dsp_xaf_record_fusionf1.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/nn/nnlib/lib/fusionf1/libxa_nnlib.a)

TARGET_LINK_LIBRARIES(dsp_xaf_record_fusionf1.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET dsp_xaf_record_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_xaf_record_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_reset_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.ResetVector.text
)

ADD_CUSTOM_COMMAND(TARGET dsp_xaf_record_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_xaf_record_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.WindowVectors.text
--only-section=.Level2InterruptVector.text
--only-section=.Level3InterruptVector.literal
--only-section=.Level3InterruptVector.text
--only-section=.DebugExceptionVector.literal
--only-section=.DebugExceptionVector.text
--only-section=.NMIExceptionVector.literal
--only-section=.NMIExceptionVector.text
--only-section=.KernelExceptionVector.text
--only-section=.UserExceptionVector.text
--only-section=.DoubleExceptionVector.text
--only-section=.text
)

ADD_CUSTOM_COMMAND(TARGET dsp_xaf_record_fusionf1.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_xaf_record_fusionf1.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt500_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.clib.rodata
--only-section=.rtos.rodata
--only-section=.rodata
--only-section=.clib.data
--only-section=.rtos.percpu.data
--only-section=.data
--only-section=.bss
)

