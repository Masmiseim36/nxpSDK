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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_DEBUG=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_MP3_DECODER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_AAC_DECODER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_VORBIS_DECODER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_OPUS_ENCODER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_OPUS_DECODER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_PCM_GAIN=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_SRC_PP_FX=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_RENDERER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_CAPTURER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXOS_CLOCK_FREQ=594000000")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAPP_DSP_ONLY=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DI2S_SUPPORT_44100_HZ=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHAVE_XOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHIFI_ONLY_XAF")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFIO_LOCAL_FS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_RX_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=nxp_rt600_RI2019_newlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-div32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mlongcalls")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -INLINE:requested")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_VORBIS_DECODER=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_OPUS_ENCODER=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_OPUS_DECODER=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_MP3_DECODER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_AAC_DECODER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_PCM_GAIN=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_SRC_PP_FX=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_RENDERER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_CAPTURER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXOS_CLOCK_FREQ=594000000")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAPP_DSP_ONLY=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DI2S_SUPPORT_44100_HZ=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHAVE_XOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHIFI_ONLY_XAF")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFIO_LOCAL_FS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_RX_ENABLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=nxp_rt600_RI2019_newlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-div32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mlongcalls")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -INLINE:requested")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-core=nxp_rt600_RI2019_newlib")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-core=nxp_rt600_RI2019_newlib")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT685S/xtensa/gdbio")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-core=nxp_rt600_RI2019_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT685S/xtensa/min-rt")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-core=nxp_rt600_RI2019_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../src/dsp_examples/xaf_demo/resources)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt600_hifi4)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/sysdeps/xos/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins)

include_directories(${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/test_inp)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities)

IF(${CMAKE_BUILD_TYPE} STREQUAL debug)
add_executable(dsp_xaf_demo_hifi4.elf 
"${ProjDirPath}/../srtm_src.c"
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../srtm_config.h"
"${ProjDirPath}/../srtm_config_audio.h"
"${ProjDirPath}/../srtm_config_nn.h"
"${ProjDirPath}/../srtm_config_tfulite.h"
"${ProjDirPath}/../sine.pcm.h"
"${ProjDirPath}/../hihat_dec_out.pcm.h"
"${ProjDirPath}/../hihat.mp3.h"
"${ProjDirPath}/../hihat_48k.mp3.h"
"${ProjDirPath}/../testvector04-48000-2ch-ogg-opus_trim.out.h"
"${ProjDirPath}/../testvector04.bit.opus.h"
"${ProjDirPath}/../testvector04-48000-2ch_trim.out.h"
"${ProjDirPath}/../testvector04.bit.h"
"${ProjDirPath}/../testvector11-16000-1ch_trim.out.h"
"${ProjDirPath}/../testvector11-16kHz-1ch-20kbps_trim.bit.h"
"${ProjDirPath}/../aac.adts.h"
"${ProjDirPath}/../hihat.ogg.h"
"${ProjDirPath}/../hihat_vorbis_dec_out_trim.pcm.h"
"${ProjDirPath}/../i9_trim_pcm.h"
"${ProjDirPath}/../i9_vor.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../xaf_main_dsp.c"
"${ProjDirPath}/../core_dsp.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_hifi4.c"
"${ProjDirPath}/../board_hifi4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_sema42.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_sema42.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi_dma.h"
"${ProjDirPath}/../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_ctimer.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_ctimer.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt600_hifi4/rpmsg_platform.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imxrt600_hifi4/rpmsg_platform.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_pint.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_pint.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_iopctl.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_bm.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_ns.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/audio/xa-class-base.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/lib/rbtree.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss/xf-board.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-shmem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-sys.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-component.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-core.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-io.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-mm.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-msg.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-plugin.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-probe.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-sched.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-timebase.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-trace.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/rbtree.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-audio-codec.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-base.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-capturer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mimo-proc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mixer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-renderer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-core.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-io.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipi.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-isr.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-main.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msg.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq1.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-sched.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-structs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-version.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proto.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xaf-api.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-proxy.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-trace.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_af_hostless_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_api_defs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-ap-dp-common.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-msgq-if.h"
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
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xaf-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xf-debug.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec22-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec23-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-encoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-gain-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-mix-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-split-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_aac_dec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_amr_wb_dec_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_mp3_dec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_mp3_enc_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_ogg_lib_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_opus_codec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_src_pp_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_vorbis_dec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/host_fio.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-clk-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-fio-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-utils-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/aac_dec/xa-aac-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_dec/xa-mp3-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus/xa-opus-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/pcm_gain/xa-pcm-gain.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/src-pp/xa-src-pp.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/vorbis_dec/xa-vorbis-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-dmic.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-i2s.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/opus/xa-opus-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/renderer/xa-renderer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/xa-factory.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/fsl_shell.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/fsl_shell.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/MIMXRT685S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/MIMXRT685S_dsp_features.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/system_MIMXRT685S_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/system_MIMXRT685S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../srtm_pcm_gain.c"
"${ProjDirPath}/../srtm_capturer_pcm_gain_renderer.c"
"${ProjDirPath}/../srtm_utils.c"
"${ProjDirPath}/../srtm_utils.h"
"${ProjDirPath}/../ringbuffer.c"
"${ProjDirPath}/../ringbuffer.h"
"${ProjDirPath}/../srtm_opus_enc.c"
"${ProjDirPath}/../srtm_decoder.c"
"${ProjDirPath}/../srtm_file_dec.c"
)
ENDIF()

IF(${CMAKE_BUILD_TYPE} STREQUAL release)
add_executable(dsp_xaf_demo_hifi4.elf 
"${ProjDirPath}/../srtm_src.c"
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../srtm_config.h"
"${ProjDirPath}/../srtm_config_audio.h"
"${ProjDirPath}/../srtm_config_nn.h"
"${ProjDirPath}/../srtm_config_tfulite.h"
"${ProjDirPath}/../sine.pcm.h"
"${ProjDirPath}/../hihat_dec_out.pcm.h"
"${ProjDirPath}/../hihat.mp3.h"
"${ProjDirPath}/../hihat_48k.mp3.h"
"${ProjDirPath}/../testvector04-48000-2ch-ogg-opus_trim.out.h"
"${ProjDirPath}/../testvector04.bit.opus.h"
"${ProjDirPath}/../testvector04-48000-2ch_trim.out.h"
"${ProjDirPath}/../testvector04.bit.h"
"${ProjDirPath}/../testvector11-16000-1ch_trim.out.h"
"${ProjDirPath}/../testvector11-16kHz-1ch-20kbps_trim.bit.h"
"${ProjDirPath}/../aac.adts.h"
"${ProjDirPath}/../hihat.ogg.h"
"${ProjDirPath}/../hihat_vorbis_dec_out_trim.pcm.h"
"${ProjDirPath}/../i9_trim_pcm.h"
"${ProjDirPath}/../i9_vor.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../xaf_main_dsp.c"
"${ProjDirPath}/../core_dsp.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_hifi4.c"
"${ProjDirPath}/../board_hifi4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2c_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_i2s_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_sema42.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_sema42.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_spi_dma.h"
"${ProjDirPath}/../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_ctimer.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_ctimer.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imxrt600_hifi4/rpmsg_platform.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imxrt600_hifi4/rpmsg_platform.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_pint.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_pint.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_dmic_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart_dma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart_dma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_iopctl.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_bm.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_ns.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg_lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/audio/xa-class-base.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/lib/rbtree.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss/xf-board.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-shmem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-sys.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-component.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-core.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-dp_proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-io.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-mm.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-msg.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-plugin.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-probe.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-sched.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-timebase.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/xf-trace.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/rbtree.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-audio-codec.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-base.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-capturer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mimo-proc.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mixer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-renderer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-core.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-io.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipi.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-isr.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-main.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msg.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq1.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-sched.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-config.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-hal.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-ipc.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-runtime.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-types.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-structs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xaf-version.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-opcode.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proto.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf-proxy.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/include/xf.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xaf-api.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-proxy.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-trace.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_af_hostless_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xa_api_defs.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-ap-dp-common.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include/xf-msgq-if.h"
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
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xaf-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/libxa_af_hostless/include/xf-debug.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec22-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-aec23-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-opus-encoder-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-gain-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-mix-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa-pcm-split-api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_aac_dec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_amr_wb_dec_definitions.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_mp3_dec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_mp3_enc_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_ogg_lib_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_opus_codec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_src_pp_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/audio/xa_vorbis_dec_api.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/host_fio.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-clk-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-fio-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-mem.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/include/xaf-utils-test.h"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/aac_dec/xa-aac-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_dec/xa-mp3-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus/xa-opus-encoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/pcm_gain/xa-pcm-gain.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/src-pp/xa-src-pp.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/vorbis_dec/xa-vorbis-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-dmic.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/capturer/xa-capturer-i2s.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/opus/xa-opus-decoder.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/nxp/renderer/xa-renderer.c"
"${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/xa-factory.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/fsl_shell.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/fsl_shell.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/MIMXRT685S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/MIMXRT685S_dsp_features.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/system_MIMXRT685S_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/system_MIMXRT685S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../srtm_pcm_gain.c"
"${ProjDirPath}/../srtm_capturer_pcm_gain_renderer.c"
"${ProjDirPath}/../srtm_utils.c"
"${ProjDirPath}/../srtm_utils.h"
"${ProjDirPath}/../ringbuffer.c"
"${ProjDirPath}/../ringbuffer.h"
"${ProjDirPath}/../srtm_opus_enc.c"
"${ProjDirPath}/../srtm_decoder.c"
"${ProjDirPath}/../srtm_file_dec.c"
)
ENDIF()


TARGET_LINK_LIBRARIES(dsp_xaf_demo_hifi4.elf -Wl,--start-group)
target_link_libraries(dsp_xaf_demo_hifi4.elf debug xtutil)

target_link_libraries(dsp_xaf_demo_hifi4.elf debug xos)

target_link_libraries(dsp_xaf_demo_hifi4.elf optimized xos)


target_link_libraries(dsp_xaf_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/aac_dec/lib/xa_aac_dec.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_dec/lib/xa_mp3_dec.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus/lib/xa_opus_codec.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/src-pp/lib/xa_src_pp.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/vorbis_dec/lib/xa_vorbis_dec.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/aac_dec/lib/xa_aac_dec.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/mp3_dec/lib/xa_mp3_dec.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/opus/lib/xa_opus_codec.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/src-pp/lib/xa_src_pp.a)


target_link_libraries(dsp_xaf_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/dsp/audio_framework/testxa_af_hostless/test/plugins/cadence/vorbis_dec/lib/xa_vorbis_dec.a)

TARGET_LINK_LIBRARIES(dsp_xaf_demo_hifi4.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET dsp_xaf_demo_hifi4.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_xaf_demo_hifi4.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt600_RI2019_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.dram0.rodata
--only-section=.Level2InterruptVector.literal
--only-section=.Level3InterruptVector.literal
--only-section=.DebugExceptionVector.literal
--only-section=.NMIExceptionVector.literal
--only-section=.KernelExceptionVector.literal
--only-section=.UserExceptionVector.literal
--only-section=.DoubleExceptionVector.literal
--only-section=.iram0.literal
--only-section=.ResetVector.literal
--only-section=.dram0.literal
--only-section=.dram0.data
--only-section=.dram0.bss
--only-section=.ResetVector.text
--only-section=.ResetHandler.literal
--only-section=.ResetHandler.text
--only-section=.WindowVectors.text
--only-section=.Level2InterruptVector.text
--only-section=.Level3InterruptVector.text
--only-section=.DebugExceptionVector.text
--only-section=.NMIExceptionVector.text
--only-section=.KernelExceptionVector.text
--only-section=.UserExceptionVector.text
--only-section=.DoubleExceptionVector.text
--only-section=.iram0.text
)

ADD_CUSTOM_COMMAND(TARGET dsp_xaf_demo_hifi4.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_xaf_demo_hifi4.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt600_RI2019_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.text
--only-section=.rodata
--only-section=.data
--only-section=.bss
--only-section=.literal
--only-section=.sram.text
--only-section=.sram.rodata
--only-section=.sram.data
--only-section=.sram.bss
--only-section=.sram.literal
--only-section=.clib.rodata
--only-section=.clib.text
--only-section=.clib.data
--only-section=.clib.percpu.data
--only-section=.clib.bss
--only-section=.clib.percpu.bss
--only-section=.clib.literal
--only-section=.rtos.rodata
--only-section=.rtos.text
--only-section=.rtos.data
--only-section=.rtos.percpu.data
--only-section=.rtos.bss
--only-section=.rtos.percpu.bss
--only-section=.rtos.literal
)

ADD_CUSTOM_COMMAND(TARGET dsp_xaf_demo_hifi4.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_xaf_demo_hifi4.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_ncache_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt600_RI2019_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=NonCacheable
)

