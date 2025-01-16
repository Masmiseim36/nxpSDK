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


project(tflm_cifar10_hifi4_hifi4)

set(MCUX_SDK_PROJECT_NAME tflm_cifar10_hifi4_hifi4)

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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXOS_CLOCK_FREQ=237500000")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUX_META_BUILD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -include ${ProjDirPath}/../mcux_config.h")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXOS_CLOCK_FREQ=237500000")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUX_META_BUILD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUX_META_BUILD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -stdlib=libc++")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-core=rt700_hifi4_RI23_11_nlib")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MIMXRT798SGFOA_hifi4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUX_META_BUILD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DTF_LITE_STATIC_MEMORY")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -stdlib=libc++")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fmessage-length=0")

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

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/ruy)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/ruy)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/ruy)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn)

include_directories(${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

add_executable(tflm_cifar10_hifi4_hifi4.elf 
"${ProjDirPath}/../get_top_n.cpp"
"${ProjDirPath}/../get_top_n.h"
"${ProjDirPath}/../model.cpp"
"${ProjDirPath}/../model.h"
"${ProjDirPath}/../demo_info.cpp"
"${ProjDirPath}/../demo_info.h"
"${ProjDirPath}/../timer_xtensa.c"
"${ProjDirPath}/../timer_xtensa.h"
"${ProjDirPath}/../timer.h"
"${ProjDirPath}/../model_data.h"
"${ProjDirPath}/../model_cifarnet_ops_micro.cpp"
"${ProjDirPath}/../main.cpp"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../image.h"
"${ProjDirPath}/../image_decode_raw.c"
"${ProjDirPath}/../image_utils.h"
"${ProjDirPath}/../image_load.c"
"${ProjDirPath}/../image_data.h"
"${ProjDirPath}/../labels.h"
"${ProjDirPath}/../ship.bmp"
"${ProjDirPath}/../demo_config.h"
"${ProjDirPath}/../output_postproc.h"
"${ProjDirPath}/../output_postproc.cpp"
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
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_iopctl.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_common_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpflexcomm.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpflexcomm.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_assert.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_uart.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_lpuart.c"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/compiler/mlir/lite/kernels/internal/compatibility_macros.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/compiler/mlir/lite/schema/schema_generated.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/compiler/mlir/lite/schema/schema_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/kernel_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/op_macros.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/padding.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/compatibility.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/cppmath.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/max.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/min.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/portable_tensor.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/portable_tensor_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/quantization_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/runtime_shape.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/strided_slice_logic.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/tensor_ctypes.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/types.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/add.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/add_n.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/arg_min_max.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/batch_matmul.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/batch_to_space_nd.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/binary_function.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/broadcast_args.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/broadcast_to.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/ceil.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/comparisons.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/concatenation.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/cumsum.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/depth_to_space.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/depthwiseconv_float.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/depthwiseconv_uint8.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/dequantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/div.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/elu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/exp.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/fill.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/floor.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/floor_div.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/floor_mod.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/fully_connected.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/hard_swish.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/l2normalization.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/leaky_relu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/log_softmax.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/logistic.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/lstm_cell.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/maximum_minimum.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/mul.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/neg.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/pad.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/pooling.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils_impl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/prelu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/process_broadcast_shapes.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/quantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/reduce.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/requantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/resize_bilinear.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/resize_nearest_neighbor.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/round.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/select.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/slice.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/softmax.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/space_to_batch_nd.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/space_to_depth.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/strided_slice.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/sub.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/tanh.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/transpose.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/transpose_conv.h"
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
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/transpose_conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/optimized/neon_check.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/bits.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/fft.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/fft_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/filterbank.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/filterbank_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/frontend.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/frontend_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/kiss_fft_common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/kiss_fft_int16.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/log_lut.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/log_scale.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/log_scale_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/noise_reduction.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/noise_reduction_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/window.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/experimental/microfrontend/lib/window_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/schema/schema_generated.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/schema/schema_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/array.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/builtin_op_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/builtin_ops.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/context_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/portable_type_to_tflitetype.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/error_reporter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/flatbuffer_conversions.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/tensor_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/macros.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/c/builtin_op_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/c/c_api_types.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/c/common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/compatibility.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/fake_micro_context.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/flatbuffer_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_helpers.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_allocation_info.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_arena_constants.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_context.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_graph.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_interpreter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_interpreter_context.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_interpreter_graph.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_log.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_mutable_op_resolver.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_op_resolver.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_profiler.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_profiler_interface.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_resource_variable.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_time.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/micro_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/mock_micro_graph.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/recording_micro_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/recording_micro_interpreter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/system_setup.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/test_helper_custom_ops.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/test_helpers.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/activation_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/activations.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/add.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/circular_buffer.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/circular_buffer_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/conv_test.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/depthwise_conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/dequantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/detection_postprocess_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/ethosu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/fully_connected.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/hard_swish.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/kernel_runner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/kernel_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/leaky_relu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/logical.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/logistic.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/lstm_eval.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/lstm_eval_test.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/lstm_shared.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/micro_ops.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/micro_tensor_utils.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/mul.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/pad.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/pooling.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/prelu.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/quantize.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/reduce.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/reshape.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/softmax.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/strided_slice.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/sub.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/svdf.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/transpose_conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/unidirectional_sequence_lstm.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/lstm_eval.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_add.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_depthwise_conv.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_fully_connected.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_pad.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_pooling.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_reduce.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_reshape.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_softmax.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/kernels/xtensa/xtensa_svdf.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/greedy_memory_planner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/linear_memory_planner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/memory_plan_struct.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/micro_memory_planner.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/arena_allocator/ibuffer_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/tflite_bridge/flatbuffer_conversions_bridge.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/builtin_op_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/c_api_types.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/delay_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/energy_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/fft_auto_scale_kernel.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/fft_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/filter_bank_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/filter_bank_log_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/filter_bank_spectral_subtraction_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/filter_bank_square_root.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/framer_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/irfft.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/overlap_add_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/pcan_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/rfft.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/stacker_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/micro/kernels/window_flexbuffers_generated_data.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/circular_buffer.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/complex.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/energy.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/fft_auto_scale.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/filter_bank.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/filter_bank_log.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/filter_bank_spectral_subtraction.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/filter_bank_square_root.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/irfft.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/log.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/max_abs.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/msb.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/overlap_add.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/pcan_argc_fixed.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/rfft.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/square_root.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/window.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/kiss_fft_wrappers/kiss_fft_common.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/kiss_fft_wrappers/kiss_fft_float.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/kiss_fft_wrappers/kiss_fft_int16.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/signal/src/kiss_fft_wrappers/kiss_fft_int32.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include/arm_nn_math_types.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include/arm_nn_tables.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include/arm_nn_types.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include/arm_nnfunctions.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include/arm_nnsupportfunctions.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include/Internal/arm_nn_compiler.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/fft2d/fft.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/fft2d/fft2d.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/array.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/base.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/buffer.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/buffer_ref.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/code_generator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/code_generators.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/default_allocator.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/detached_buffer.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/file_manager.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flatbuffer_builder.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flatbuffers.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flatc.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flex_flat_util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flexbuffers.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/grpc.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/hash.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/idl.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/minireflect.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/reflection.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/reflection_generated.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/registry.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/stl_emulation.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/string.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/struct.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/table.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/util.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/vector.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/vector_downward.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/verifier.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp/fixedpoint/fixedpoint.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp/fixedpoint/fixedpoint_neon.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp/fixedpoint/fixedpoint_sse.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/gemmlowp/internal/detect_platform.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/ruy/ruy/profiler/instrumentation.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft/_kiss_fft_guts.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft/kiss_fft.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft/tools/kfc.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft/tools/kiss_fftnd.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft/tools/kiss_fftndr.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/third_party/kissfft/tools/kiss_fftr.h"
"${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.cpp"
"${ProjDirPath}/../mcux_config.h"
)


TARGET_LINK_LIBRARIES(tflm_cifar10_hifi4_hifi4.elf -Wl,--start-group)
target_link_libraries(tflm_cifar10_hifi4_hifi4.elf debug xos)

target_link_libraries(tflm_cifar10_hifi4_hifi4.elf optimized xos)


target_link_libraries(tflm_cifar10_hifi4_hifi4.elf debug ${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/lib/hifi4/xcc/rt798s/libtflm.a)


target_link_libraries(tflm_cifar10_hifi4_hifi4.elf optimized ${ProjDirPath}/../../../../../../middleware/eiq/tensorflow-lite/lib/hifi4/xcc/rt798s/libtflm.a)

TARGET_LINK_LIBRARIES(tflm_cifar10_hifi4_hifi4.elf -Wl,--end-group)

add_custom_command(TARGET tflm_cifar10_hifi4_hifi4.elf POST_BUILD
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_literal_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi4_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.Level3InterruptVector.literal --only-section=.DebugExceptionVector.literal --only-section=.NMIExceptionVector.literal --only-section=.UserExceptionVector.literal
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi4_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.ResetVector.text --only-section=.WindowVectors.text --only-section=.Level2InterruptVector.text --only-section=.Level3InterruptVector.text --only-section=.DebugExceptionVector.text --only-section=.NMIExceptionVector.text --only-section=.KernelExceptionVector.text --only-section=.UserExceptionVector.text --only-section=.DoubleExceptionVector.text
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi4_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.clib.rodata --only-section=.rtos.rodata --only-section=.rodata --only-section=.text --only-section=.clib.data --only-section=.rtos.percpu.data --only-section=.data --only-section=.bss
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.bin
)

