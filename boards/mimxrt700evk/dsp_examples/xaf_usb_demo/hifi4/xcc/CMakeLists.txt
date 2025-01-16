# CROSS COMPILER SETTING
set(CMAKE_SYSTEM_NAME Generic)
cmake_minimum_required(VERSION 2.6)

# THE VERSION NUMBER
set(Tutorial_VERSION_MAJOR 1)
set(Tutorial_VERSION_MINOR 0)

# ENABLE ASM
enable_language(ASM)

set(CMAKE_STATIC_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_SUFFIX)

set(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
set(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
set(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(dsp_xaf_usb_demo_hifi4)

set(MCUX_SDK_PROJECT_NAME dsp_xaf_usb_demo_hifi4)

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -include ${ProjDirPath}/../mcux_config.h")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_DEBUG=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_PCM_GAIN=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_CAPTURER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXA_RENDERER=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXOS_CLOCK_FREQ=230000000")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_CORE_ID=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXF_CFG_CORES_NUM=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DRENDERER_SAI")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCAPTURER_PDM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFIO_LOCAL_FS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHAVE_XOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHIFI_ONLY_XAF")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUX_META_BUILD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-mul32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-div32")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mlongcalls")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -INLINE:requested")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -include ${ProjDirPath}/../mcux_config.h")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_PCM_GAIN=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_CAPTURER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXA_RENDERER=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXOS_CLOCK_FREQ=230000000")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXF_CORE_ID=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXF_CFG_CORES_NUM=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRENDERER_SAI")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCAPTURER_PDM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFIO_LOCAL_FS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHAVE_XOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHIFI_ONLY_XAF")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUX_META_BUILD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-mul32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-div32")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mlongcalls")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -INLINE:requested")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUX_META_BUILD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUX_META_BUILD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mlsp=${CMAKE_CURRENT_LIST_DIR}/../../../../../../devices/MIMXRT798S/xtensa/hifi4/gdbio")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--orphan-handling=place")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mlsp=${CMAKE_CURRENT_LIST_DIR}/../../../../../../devices/MIMXRT798S/xtensa/hifi4/min-rt")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--orphan-handling=place")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/environment/xos)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/platform/imxrt700_hifi4)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/aacplus_dec/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/asrc_src_pp/hifi4/include/src_pp)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/bsac/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/dabplus/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/drm/hifi4/include/drm_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/g711/hifi4/include/g711_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/g722/hifi4/include/g722_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_dec/hifi4/include/mp3_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/include/opus_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/include/ogg_lib)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_dec/hifi4/include/sbc_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_enc/hifi4/include/sbc_enc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/vorbis_dec/hifi4/include/vorbis_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/lib)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/xos/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/mc_ipc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/vit_pre_proc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/voice_seeker)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/xf_shared/include)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/environment/xos)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/platform/imxrt700_hifi4)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/aacplus_dec/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/asrc_src_pp/hifi4/include/src_pp)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/bsac/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/dabplus/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/drm/hifi4/include/drm_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/g711/hifi4/include/g711_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/g722/hifi4/include/g722_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_dec/hifi4/include/mp3_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/include/opus_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/include/ogg_lib)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_dec/hifi4/include/sbc_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_enc/hifi4/include/sbc_enc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/vorbis_dec/hifi4/include/vorbis_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/lib)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/xos/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/mc_ipc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/vit_pre_proc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/voice_seeker)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/xf_shared/include)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/environment/xos)

include_directories(${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/platform/imxrt700_hifi4)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/aacplus_dec/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/asrc_src_pp/hifi4/include/src_pp)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/bsac/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/dabplus/hifi4/include/aac_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/drm/hifi4/include/drm_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/g711/hifi4/include/g711_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/g722/hifi4/include/g722_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_dec/hifi4/include/mp3_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/include/opus_codec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/include/ogg_lib)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_dec/hifi4/include/sbc_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_enc/hifi4/include/sbc_enc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/codecs/vorbis_dec/hifi4/include/vorbis_dec)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/lib)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/xos/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/mc_ipc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/vit_pre_proc)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/voice_seeker)

include_directories(${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/xf_shared/include)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

add_executable(dsp_xaf_usb_demo_hifi4.elf 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../ringbuffer.c"
"${ProjDirPath}/../srtm_UsbMic.c"
"${ProjDirPath}/../srtm_UsbSpeaker.c"
"${ProjDirPath}/../srtm_utils.c"
"${ProjDirPath}/../ringbuffer.h"
"${ProjDirPath}/../srtm_utils.h"
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../dsp_support.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../srtm_config.h"
"${ProjDirPath}/../srtm_config_audio.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_ADC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_AHBSC0.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_AHBSC3.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_AHBSC4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CACHE64_CTRL.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CACHE64_POLSEL.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CDOG.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CLKCTL0.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CLKCTL1.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CLKCTL2.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CLKCTL3.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CLKCTL4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CMP.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CRC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_CTIMER.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_DEBUG_MAILBOX.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_DMA.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_ELS.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_FLEXIO.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_FREQME.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_FRO.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_GDET.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_GLIKEY.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_GPIO.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_I2S.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_I3C.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_INPUTMUX0.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_INPUTMUX1.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_IOPCTL0.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_IOPCTL1.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_IOPCTL2.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_ITRC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_JPEGDEC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_JPGDECWRP.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_LCDIF.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_LPI2C.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_LPSPI.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_LPUART.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_LP_FLEXCOMM.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_MIPI_DSI_HOST.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_MMU.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_MRT.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_MU.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_NIC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_OCOTP.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_OSC32KNP.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_OSCCA.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_OSTIMER.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_PDM.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_PINT.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_PKC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_PMC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_PNGDEC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_PUF.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_PVTS.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_ROMCP.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_RSTCTL0.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_RSTCTL1.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_RSTCTL2.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_RSTCTL3.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_RSTCTL4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_RTC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SCT.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SDADC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SEMA42.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SLEEPCON0.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SLEEPCON1.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SYSCON0.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SYSCON1.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SYSCON2.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SYSCON3.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SYSCON4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_SYSPM.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_TRNG.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_USB.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_USBHSDCD.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_USBNC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_USBPHY.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_USDHC.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_UTICK.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_WWDT.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_XCACHE.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph/PERI_XSPI.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/MIMXRT798S_hifi4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/MIMXRT798S_hifi4_features.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/MIMXRT798S_hifi4_COMMON.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/system_MIMXRT798S_hifi4.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/system_MIMXRT798S_hifi4.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_edma_soc.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_edma_soc.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_inputmux_connections.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_iopctl.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_edma_core.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpflexcomm.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpflexcomm.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_pdm_edma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_pdm_edma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_pdm.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_pdm.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sai_edma.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sai_edma.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sai.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sai.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sema42.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sema42.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_assert.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_uart.h"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_uart.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_lpuart.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/rpmsg_queue.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/rpmsg_lite/rpmsg_queue.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/environment/xos/rpmsg_env_specific.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/rpmsg_lite/porting/environment/rpmsg_env_xos.c"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/include/platform/imxrt700_hifi4/rpmsg_platform.h"
"${ProjDirPath}/../../../../../../middleware/multicore/rpmsg-lite/lib/rpmsg_lite/porting/platform/imxrt700_hifi4/rpmsg_platform.c"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/lc3/hifi4/include/xa_error_handler.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/lc3/hifi4/include/xa_error_standards.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/lc3/hifi4/include/xa_type_def.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/lc3/hifi4/include/lc3_codec/xa_lc3_codec_api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/include/xa_apicmd_standards.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/include/xa_error_handler.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/include/xa_error_standards.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/include/xa_memory_standards.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/include/xa_type_def.h"
"${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/include/mp3_enc/xa_mp3_enc_api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/rbtree.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xa-class-audio-codec.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xa-class-base.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xa-class-capturer.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xa-class-mimo-proc.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xa-class-mixer.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xa-class-renderer.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-core.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-io.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-ipc-if.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-ipi.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-isr.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-main.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-mem-ipc.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-mem.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-msg.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-msgq.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/src/xf-sched.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-component.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-core.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-dp.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-dp_opcode.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-dp_proxy.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-flx-lock-if.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-io.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-mm.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-msg.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-plugin.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-probe.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-sched.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-timebase.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/xf-trace.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/audio/xa-class-base.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/lib/rbtree.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_config.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_hal.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_ipc.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_runtime.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-dp_types.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-ipc-if.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem-ipc.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-mem.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-shmem.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/xf-sys.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss/xf-board.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/src/xaf-api.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/src/xf-msgq1.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/src/xf-proxy.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/src/xf-trace.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/xaf-structs.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/xaf-threads-priority.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/xaf-version.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/xf-opcode.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/xf-proto.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/xf-proxy.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/xf.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-config.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-hal.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-ipc.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-runtime.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/host-apf/include/sys/xos-msgq/xf-types.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/xa_af_hostless/include/xa_af_hostless_definitions.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/xa_af_hostless/include/xa_api_defs.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/xa_af_hostless/include/xf-ap-dp-common.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/algo/xa_af_hostless/include/xf-msgq-if.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/xaf-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/xaf-config-user.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/xf-debug.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa-audio-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa-capturer-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa-mimo-proc-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa-mixer-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa-renderer-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa_apicmd_standards.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa_error_standards.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa_memory_standards.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/audio/xa_type_def.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/freertos/include/osal-isr.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/freertos/include/osal-msgq.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/freertos/include/osal-thread.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/freertos/include/osal-timer.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/mc_ipc/xf-mc-ipc.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/xos/include/osal-isr.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/xos/include/osal-msgq.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/xos/include/osal-thread.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/include/sysdeps/xos/include/osal-timer.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/host_fio.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/xaf-app-threads-priority.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/xaf-clk-test.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/xaf-fio-test.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/xaf-mem.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/xaf-test.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/xaf-utils-test.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-aec22-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-aec23-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-microspeech-frontend-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-microspeech-inference-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-mimo-mix-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-opus-decoder-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-opus-encoder-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-pcm-gain-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-pcm-split-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-pcm-stereo-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa-person-detect-inference-api.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/include/audio/xa_amr_wb_dec_definitions.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/xa-gain-factor-event.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/xa-factory.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/aac_dec/xa-aac-decoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/aec22/xa-aec22.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/aec23/xa-aec23.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/mixer/xa-mixer.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/mp3_dec/xa-mp3-decoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/mp3_enc/xa-mp3-encoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/opus_dec/opus_header.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/opus_dec/xa-opus-decoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/opus_dec/opus_header.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/opus_enc/xa-opus-encoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/pcm_gain/xa-pcm-gain.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/pcm_split/xa-pcm-split.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/src-pp/xa-src-pp.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/cadence/vorbis_dec/xa-vorbis-decoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/capturer/xa-capturer-dmic.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/capturer/xa-capturer-i2s.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/capturer/xa-capturer-pdm.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/pcm_stereo/xa-pcm-stereo.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/renderer/xa-renderer-sai.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/renderer/xa-renderer.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/sbc_dec/xa-sbc-decoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/sbc_enc/xa-sbc-encoder.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.h"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/src/xaf-fio-test.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/test/src/xaf-mem-test.c"
"${ProjDirPath}/../../../../../../middleware/cadence/multicore-xaf/xa_af_hostless/xf_shared/include/xf-shared.h"
"${ProjDirPath}/../mcux_config.h"
)


TARGET_LINK_LIBRARIES(dsp_xaf_usb_demo_hifi4.elf -Wl,--start-group)
target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug xtutil)

target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug xos)

target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized xos)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/aacplus_dec/hifi4/lib/xa_aacplus_v2_loas_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/asrc_src_pp/hifi4/lib/xa_asrc_src_pp.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/bsac/hifi4/lib/xa_bsac_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/dabplus/hifi4/lib/xa_dabplus_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/drm/hifi4/lib/xa_drm_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/g711/hifi4/lib/xa_g711_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/g722/hifi4/lib/xa_g722_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_dec/hifi4/lib/xa_mp3_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/xa_opus_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/xa_opus_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/xa_opus_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/silk/xa_opus_silk_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/silk/xa_opus_silk_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/silk/xa_opus_silk_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/celt/xa_opus_celt_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/celt/xa_opus_celt_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/celt/xa_opus_celt_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_dec/hifi4/lib/xa_sbc_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_enc/hifi4/lib/xa_sbc_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/vorbis_dec/hifi4/lib/xa_vorbis_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/lc3/hifi4/lib/xa_lc3_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/lib/xa_mp3_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/aacplus_dec/hifi4/lib/xa_aacplus_v2_loas_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/asrc_src_pp/hifi4/lib/xa_asrc_src_pp.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/bsac/hifi4/lib/xa_bsac_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/dabplus/hifi4/lib/xa_dabplus_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/drm/hifi4/lib/xa_drm_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/g711/hifi4/lib/xa_g711_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/g722/hifi4/lib/xa_g722_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_dec/hifi4/lib/xa_mp3_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/xa_opus_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/xa_opus_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/xa_opus_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/silk/xa_opus_silk_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/silk/xa_opus_silk_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/silk/xa_opus_silk_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/celt/xa_opus_celt_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/celt/xa_opus_celt_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/opus/hifi4/lib/celt/xa_opus_celt_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_dec/hifi4/lib/xa_sbc_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/sbc_enc/hifi4/lib/xa_sbc_enc.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/vorbis_dec/hifi4/lib/xa_vorbis_dec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/lc3/hifi4/lib/xa_lc3_codec.a)


target_link_libraries(dsp_xaf_usb_demo_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/cadence/codecs/mp3_enc/hifi4/lib/xa_mp3_enc.a)

TARGET_LINK_LIBRARIES(dsp_xaf_usb_demo_hifi4.elf -Wl,--end-group)

add_custom_command(TARGET dsp_xaf_usb_demo_hifi4.elf POST_BUILD
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_literal_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi4_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.Level3InterruptVector.literal --only-section=.DebugExceptionVector.literal --only-section=.NMIExceptionVector.literal --only-section=.UserExceptionVector.literal
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi4_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.ResetVector.text --only-section=.WindowVectors.text --only-section=.Level2InterruptVector.text --only-section=.Level3InterruptVector.text --only-section=.DebugExceptionVector.text --only-section=.NMIExceptionVector.text --only-section=.KernelExceptionVector.text --only-section=.UserExceptionVector.text --only-section=.DoubleExceptionVector.text
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi4_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.clib.rodata --only-section=.rtos.rodata --only-section=.rodata --only-section=.text --only-section=.clib.data --only-section=.rtos.percpu.data --only-section=.data --only-section=.bss
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.bin
)

