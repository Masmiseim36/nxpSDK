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


project(dsp_sema42_hifi1)

set(MCUX_SDK_PROJECT_NAME dsp_sema42_hifi1)

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -include ${ProjDirPath}/../mcux_config.h")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUX_META_BUILD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT798SGFOA_hifi1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -include ${ProjDirPath}/../mcux_config.h")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUX_META_BUILD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT798SGFOA_hifi1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUX_META_BUILD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MIMXRT798SGFOA_hifi1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUX_META_BUILD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MIMXRT798SGFOA_hifi1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mlsp=${CMAKE_CURRENT_LIST_DIR}/../../../../../../devices/MIMXRT798S/xtensa/hifi1/gdbio")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--orphan-handling=place")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mlsp=${CMAKE_CURRENT_LIST_DIR}/../../../../../../devices/MIMXRT798S/xtensa/hifi1/min-rt")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--orphan-handling=place")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-core=rt700_hifi1_RI23_11_nlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/periph)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../devices/periph)

add_executable(dsp_sema42_hifi1.elf 
"${ProjDirPath}/../main.c"
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
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/MIMXRT798S_hifi1.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/MIMXRT798S_hifi1_features.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/MIMXRT798S_hifi1_COMMON.h"
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
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sema42.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/drivers/fsl_sema42.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_assert.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT798S/utilities/debug_console_lite/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_uart.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_lpuart.c"
"${ProjDirPath}/../mcux_config.h"
)



add_custom_command(TARGET dsp_sema42_hifi1.elf POST_BUILD
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi1_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.text
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_vector_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi1_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.ResetVector.text --only-section=.WindowVectors.text --only-section=.Level2InterruptVector.text --only-section=.Level3InterruptVector.text --only-section=.DebugExceptionVector.text --only-section=.NMIExceptionVector.text --only-section=.KernelExceptionVector.text --only-section=.UserExceptionVector.text --only-section=.DoubleExceptionVector.text --only-section=.Level3InterruptVector.literal --only-section=.DebugExceptionVector.literal --only-section=.NMIExceptionVector.literal --only-section=.UserExceptionVector.literal
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin --xtensa-core=rt700_hifi1_RI23_11_nlib --xtensa-system=${XTENSA_SYSTEM} --only-section=.clib.rodata --only-section=.rtos.rodata --only-section=.rodata --only-section=.clib.data --only-section=.rtos.percpu.data --only-section=.data --only-section=.bss
COMMAND ${CMAKE_OBJCOPY} --xtensa-params= -Obinary ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.elf ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${MCUX_SDK_PROJECT_NAME}.bin
)

