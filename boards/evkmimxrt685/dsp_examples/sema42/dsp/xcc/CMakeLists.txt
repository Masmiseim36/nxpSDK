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


project(dsp_sema42_hifi4)

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-core=nxp_rt600_RI2021_8_newlib")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-core=nxp_rt600_RI2021_8_newlib")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-core=nxp_rt600_RI2021_8_newlib")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsigned-char")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MIMXRT685SFVKB_dsp")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMCUXPRESSO_SDK")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-core=nxp_rt600_RI2021_8_newlib")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsigned-char")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-missing-braces")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fmessage-length=0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT685S/xtensa/gdbio")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-core=nxp_rt600_RI2021_8_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --xtensa-system=${XTENSA_SYSTEM}")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mlsp=${ProjDirPath}/../../../../../../devices/MIMXRT685S/xtensa/min-rt")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-core=nxp_rt600_RI2021_8_newlib")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --xtensa-system=${XTENSA_SYSTEM}")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console_lite)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../devices/MIMXRT685S)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Core/Include)

add_executable(dsp_sema42_hifi4.elf 
"${ProjDirPath}/../main_dsp.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_hifi4.c"
"${ProjDirPath}/../board_hifi4.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_uart.h"
"${ProjDirPath}/../../../../../../components/uart/fsl_adapter_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console_lite/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console_lite/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities/debug_console_lite/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_common_dsp.h"
"${ProjDirPath}/../../../../../../components/lists/fsl_component_generic_list.h"
"${ProjDirPath}/../../../../../../components/lists/fsl_component_generic_list.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_iopctl.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_sema42.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_sema42.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/MIMXRT685S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/MIMXRT685S_dsp_features.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/system_MIMXRT685S_dsp.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/system_MIMXRT685S_dsp.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.c"
)



ADD_CUSTOM_COMMAND(TARGET dsp_sema42_hifi4.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_sema42_hifi4.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_text_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt600_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.Level3InterruptVector.literal
--only-section=.DebugExceptionVector.literal
--only-section=.NMIExceptionVector.literal
--only-section=.UserExceptionVector.literal
--only-section=.ResetVector.text
--only-section=.WindowVectors.text
--only-section=.Level2InterruptVector.text
--only-section=.Level3InterruptVector.text
--only-section=.DebugExceptionVector.text
--only-section=.NMIExceptionVector.text
--only-section=.KernelExceptionVector.text
--only-section=.UserExceptionVector.text
--only-section=.DoubleExceptionVector.text
)

ADD_CUSTOM_COMMAND(TARGET dsp_sema42_hifi4.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
--xtensa-params= -Obinary ${EXECUTABLE_OUTPUT_PATH}/dsp_sema42_hifi4.elf ${EXECUTABLE_OUTPUT_PATH}/../../binary/dsp_data_${CMAKE_BUILD_TYPE}.bin
--xtensa-core=nxp_rt600_RI2021_8_newlib
--xtensa-system=${XTENSA_SYSTEM}
--only-section=.clib.rodata
--only-section=.rtos.rodata
--only-section=.rodata
--only-section=.text
--only-section=.clib.data
--only-section=.rtos.percpu.data
--only-section=.data
--only-section=.bss
)

