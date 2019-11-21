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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMX8MM6DVTLZ")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNOT_CONFIG_CLK_ROOT=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCODEC_AK4497_ENABLE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMX8MM6DVTLZ")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNOT_CONFIG_CLK_ROOT=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCODEC_AK4497_ENABLE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MIMX8MM6DVTLZ")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MIMX8MM6DVTLZ")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers)

include_directories(${ProjDirPath}/../../../../../components/srtm/channels)

include_directories(${ProjDirPath}/../../../../../components/srtm/services)

include_directories(${ProjDirPath}/../../../../../components/codec)

include_directories(${ProjDirPath}/../../../../../components/codec/port)

include_directories(${ProjDirPath}/../../../../../components/codec/ak4497)

include_directories(${ProjDirPath}/../../../../../components/i2c)

include_directories(${ProjDirPath}/../../../../../devices/MIMX8MM6)

include_directories(${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../components/codec/i2c)

include_directories(${ProjDirPath}/../../../../../components/srtm/include)

include_directories(${ProjDirPath}/../../../../../components/srtm/srtm)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imx8mm_m4)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include)

add_executable(sai_low_power_audio.elf 
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../lpm.c"
"${ProjDirPath}/../lpm.h"
"${ProjDirPath}/../sdma_multi_fifo_script.h"
"${ProjDirPath}/../sai_low_power_audio.c"
"${ProjDirPath}/../sai_low_power_audio.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_gpio.h"
"${ProjDirPath}/../app_srtm.c"
"${ProjDirPath}/../app_srtm.h"
"${ProjDirPath}/../../../../../components/srtm/channels/srtm_rpmsg_endpoint.c"
"${ProjDirPath}/../../../../../components/srtm/channels/srtm_rpmsg_endpoint.h"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_audio_service.c"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_audio_service.h"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_i2c_codec_adapter.c"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_i2c_codec_adapter.h"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_sai_sdma_adapter.c"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_sai_sdma_adapter.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_gpc.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_gpc.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_gpt.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_gpt.h"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.c"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.h"
"${ProjDirPath}/../../../../../components/codec/port/fsl_codec_adapter.h"
"${ProjDirPath}/../../../../../components/codec/port/ak4497/fsl_codec_adapter.c"
"${ProjDirPath}/../../../../../components/i2c/i2c.h"
"${ProjDirPath}/../../../../../components/i2c/ii2c_adapter.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_sai.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_sai.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_sai_sdma.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_sai_sdma.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_rdc.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_rdc.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/MIMX8MM6_cm4.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/MIMX8MM6_cm4_features.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/system_MIMX8MM6_cm4.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/system_MIMX8MM6_cm4.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../components/uart/iuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/gcc/startup_MIMX8MM6_cm4.S"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_sdma.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_sdma.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../components/codec/ak4497/fsl_ak4497.c"
"${ProjDirPath}/../../../../../components/codec/ak4497/fsl_ak4497.h"
"${ProjDirPath}/../../../../../components/codec/i2c/fsl_codec_i2c.c"
"${ProjDirPath}/../../../../../components/codec/i2c/fsl_codec_i2c.h"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_heap_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_mutex_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_sem_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_channel.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_defs.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_dispatcher.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_message.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_peercore.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_service.h"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_message_pool.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_channel.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_channel_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_dispatcher.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_dispatcher_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_heap.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_list.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message_pool.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_mutex.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_peercore.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_peercore_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_sem.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_service.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_service_struct.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_i2c_freertos.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_i2c_freertos.h"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../devices/MIMX8MM6/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_prototypes.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/threading_alt.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/timers.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imx8mm_m4/rpmsg_platform.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imx8mm_m4/rpmsg_platform.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_queue.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_freertos.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_queue.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_ns.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_4.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MIMX8MM6xxxxx_cm4_lpa_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MIMX8MM6xxxxx_cm4_lpa_ram.ld -static")

TARGET_LINK_LIBRARIES(sai_low_power_audio.elf -Wl,--start-group)
target_link_libraries(sai_low_power_audio.elf debug m)

target_link_libraries(sai_low_power_audio.elf debug c)

target_link_libraries(sai_low_power_audio.elf debug gcc)

target_link_libraries(sai_low_power_audio.elf debug nosys)

target_link_libraries(sai_low_power_audio.elf optimized m)

target_link_libraries(sai_low_power_audio.elf optimized c)

target_link_libraries(sai_low_power_audio.elf optimized gcc)

target_link_libraries(sai_low_power_audio.elf optimized nosys)

TARGET_LINK_LIBRARIES(sai_low_power_audio.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET sai_low_power_audio.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/sai_low_power_audio.elf ${EXECUTABLE_OUTPUT_PATH}/sai_low_power_audio.bin)
