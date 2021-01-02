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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMISRAC_DISABLE=/**/")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMISRAC_ENABLE=/**/")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MKV11Z128VLF7")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=soft")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMISRAC_DISABLE=/**/")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMISRAC_ENABLE=/**/")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MKV11Z128VLF7")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMISRAC_DISABLE=/**/")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMISRAC_ENABLE=/**/")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=soft")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMISRAC_DISABLE=/**/")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMISRAC_ENABLE=/**/")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=soft")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

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

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/aio)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/clock)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/dio)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/tsi)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/wdog)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/compiler)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/flash)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/programCounter)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/ram)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/register)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/stack)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/aio)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/clock)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/dio)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/tsi)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/wdog)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/compiler)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/flash)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/programCounter)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/ram)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/register)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/stack)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0)

include_directories(${ProjDirPath}/)

include_directories(${ProjDirPath}/../periph_init)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../middleware/motor_control/fmstr_cfg_uart)

include_directories(${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers)

include_directories(${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe)

include_directories(${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_state_machine)

include_directories(${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/state_machine)

include_directories(${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB)

include_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/aio)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/clock)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/dio)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/tsi)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/wdog)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/compiler)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/flash)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/programCounter)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/ram)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/register)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/stack)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0)

include_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0)

include_directories(${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/safety_routines)

include_directories(${ProjDirPath}/../../../../../../middleware/motor_control/boards/hvpkv11z75m/mc_pmsm/pmsm_safe/startup)

include_directories(${ProjDirPath}/../../../../../../devices/MKV11Z7)

include_directories(${ProjDirPath}/../../../../../../middleware/freemaster/src/common)

include_directories(${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial)

include_directories(${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/can)

include_directories(${ProjDirPath}/../../../../../../middleware/freemaster/src/platforms/gen32le)

include_directories(${ProjDirPath}/../../../../../../devices/MKV11Z7/utilities)

add_executable(mc_pmsm_safe.elf 
"${ProjDirPath}/../mcux/startup/startup_mcux.c"
"${ProjDirPath}/../mcux/startup/vectors_mcux.c"
"${ProjDirPath}/../mcux/startup/vectors_mcux.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers/mcdrv_adc_adc16.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers/mcdrv_adc_adc16.c"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers/mcdrv_pwm3ph_ftm.c"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers/mcdrv_pwm3ph_ftm.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers/mcdrv_gpio.c"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers/mcdrv_gpio.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_state_machine/m1_state_machine.c"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_state_machine/m1_state_machine.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_state_machine/m1_state_routines.c"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_state_machine/m1_state_routines.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/freemaster_tsa_pmsm.c"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/freemaster_tsa_pmsm.h"
"${ProjDirPath}/../hardware_cfg.h"
"${ProjDirPath}/../safety_cfg.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../main.h"
"${ProjDirPath}/../m1_pmsm_appconfig.h"
"${ProjDirPath}/../periph_init/safety_periph_init.c"
"${ProjDirPath}/../periph_init/safety_periph_init.h"
"${ProjDirPath}/../m1_mcdrv.h"
"${ProjDirPath}/../periph_init/app_periph_init.c"
"${ProjDirPath}/../periph_init/app_periph_init.h"
"${ProjDirPath}/../periph_init/mcdrv_periph_init.c"
"${ProjDirPath}/../periph_init/mcdrv_periph_init.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/AMCLIB_AngleTrackObsrv_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/AMCLIB_CtrlFluxWkng_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/AMCLIB_PMSMBemfObsrvABHw_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/AMCLIB_PMSMBemfObsrvAB_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/AMCLIB_PMSMBemfObsrvDQHw_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/AMCLIB_PMSMBemfObsrvDQ_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/AMCLIB_TrackObsrv_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/amclib.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/Include/amclib_types.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include/GDFLIB_FilterExp_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include/GDFLIB_FilterIIR1_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include/GDFLIB_FilterIIR2_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include/GDFLIB_FilterMA_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include/GDFLIB_FilterMA_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include/gdflib.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/Include/gdflib_types.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_AtanYXHw_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_AtanYX_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Atan_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_CtrlBetaIPp_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_CtrlPIp_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_DFlexRampHw_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_DFlexRamp_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_DRamp_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_DRamp_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_FlexRampHw_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_FlexRamp_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Hyst_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Integrator_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Limit_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Limit_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_LowerLimit_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_LowerLimit_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_LutPer_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_LutPer_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Lut_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Lut_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Ramp_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Ramp_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Ramp_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Ramp_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_SinCos_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_SqrtHw_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_SqrtHw_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_SqrtHw_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_SqrtHw_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Sqrt_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_Sqrt_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_UpperLimit_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_UpperLimit_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_VectorLimit1Hw_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/GFLIB_VectorLimit1_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/gflib.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/Include/gflib_types.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_Clark_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_Decoupling_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_ElimDcBusRipHw_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_ElimDcBusRipHw_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_ElimDcBusRip_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_ElimDcBusRip_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_Park_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/GMCLIB_Svm_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/gmclib.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/Include/gmclib_types.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Abs_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Abs_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Abs_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Abs_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add4_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add4_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add4_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add4_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Add_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_BiShift_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_BiShift_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_BiShift_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_BiShift_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Clb_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Clb_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Clb_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Conv_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Div1Q_A32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Div1Q_F16_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Div1Q_F32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_DivHw1Q_A32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_DivHw1Q_F16_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_DivHw1Q_F32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_DivHw_A32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_DivHw_F16_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_DivHw_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_DivHw_F32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Div_A32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Div_F16_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Div_F32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Log2_U16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MMDVSQ.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac4Rnd_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac4Rnd_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac4Rnd_F32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac4_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MacRnd_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MacRnd_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MacRnd_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MacRnd_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MacRnd_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MacRnd_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mac_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MnacRnd_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MnacRnd_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MnacRnd_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MnacRnd_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MnacRnd_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MnacRnd_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mnac_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mnac_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mnac_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mnac_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mnac_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu4Rnd_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu4Rnd_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu4Rnd_F32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu4_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MsuRnd_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MsuRnd_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MsuRnd_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MsuRnd_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MsuRnd_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MsuRnd_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Msu_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MulRnd_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MulRnd_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MulRnd_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MulRnd_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MulRnd_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_MulRnd_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mul_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mul_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mul_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mul_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mul_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Mul_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Neg_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Neg_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Neg_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Neg_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Rcp1Q_A32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_RcpHw1Q_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_RcpHw_A32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Rcp_A32_Asm.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Rnd_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Rnd_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sat_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sat_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Shift_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Shift_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Shift_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Shift_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sign_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sign_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub4_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub4_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub4_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub4_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub_A32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub_A32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub_F16.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub_F16_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub_F32.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/MLIB_Sub_F32_Asmi.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/RTCESL_cfg.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/asm_mac.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/mlib.h"
"${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/Include/mlib_types.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/system_MKV11Z7.c"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/system_MKV11Z7.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/safety_routines/safety_error_handler.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/safety_routines/safety_flow_check.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/safety_routines/safety_routines.c"
"${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/safety_routines/safety_routines.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/aio/iec60730b_aio.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/clock/iec60730b_clock.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/dio/iec60730b_dio.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/dio/iec60730b_dio_ext.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/tsi/iec60730b_tsi.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/wdog/iec60730b_wdog.c"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/common_test/wdog/iec60730b_wdog.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/compiler/asm_mac_common.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/flash/iec60730b_cm0_flash.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/iec60730b_core.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/programCounter/iec60730b_cm0_pc.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/programCounter/iec60730b_cm0_pc_object.S"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/ram/iec60730b_cm0_ram.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/register/iec60730b_cm0_reg.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/core_test/cm0/stack/iec60730b_cm0_stack.h"
"${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/iec60730b.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/MKV11Z7.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/MKV11Z7_features.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_appcmd.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_can.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_can.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_defcfg.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_pdbdm.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_pipes.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_private.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_protocol.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_protocol.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_rec.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_rec.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_scope.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_serial.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_serial.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_sha.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_tsa.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_tsa.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_ures.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_ures.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_utils.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/common/freemaster_utils.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/can/freemaster_flexcan.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/can/freemaster_flexcan.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/can/freemaster_mcan.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/can/freemaster_mcan.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/can/freemaster_mscan.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/can/freemaster_mscan.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_lpsci.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_lpsci.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_lpuart.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_lpuart.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_miniusart.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_miniusart.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_uart.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_uart.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_usart.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_usart.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_usb.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/drivers/mcuxsdk/serial/freemaster_serial_usb.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/platforms/gen32le/freemaster_gen32le.h"
"${ProjDirPath}/../../../../../../devices/MKV11Z7/utilities/fsl_sbrk.c"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_cm0plus.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/safety_hvpkv11z75m.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/safety_hvpkv11z75m.ld -static")

TARGET_LINK_LIBRARIES(mc_pmsm_safe.elf -Wl,--start-group)
target_link_libraries(mc_pmsm_safe.elf debug m)

target_link_libraries(mc_pmsm_safe.elf debug c)

target_link_libraries(mc_pmsm_safe.elf debug gcc)

target_link_libraries(mc_pmsm_safe.elf debug nosys)

target_link_libraries(mc_pmsm_safe.elf optimized m)

target_link_libraries(mc_pmsm_safe.elf optimized c)

target_link_libraries(mc_pmsm_safe.elf optimized gcc)

target_link_libraries(mc_pmsm_safe.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB)

target_link_libraries(mc_pmsm_safe.elf debug ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/libAMCLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB)

target_link_libraries(mc_pmsm_safe.elf debug ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/libGDFLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB)

target_link_libraries(mc_pmsm_safe.elf debug ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/libGFLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB)

target_link_libraries(mc_pmsm_safe.elf debug ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/libGMCLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB)

target_link_libraries(mc_pmsm_safe.elf debug ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/libMLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0)

target_link_libraries(mc_pmsm_safe.elf debug ${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/libIEC60730_CM0_Class_B_MCUX_v4_0.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB)

target_link_libraries(mc_pmsm_safe.elf optimized ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/AMCLIB/libAMCLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB)

target_link_libraries(mc_pmsm_safe.elf optimized ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GDFLIB/libGDFLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB)

target_link_libraries(mc_pmsm_safe.elf optimized ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GFLIB/libGFLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB)

target_link_libraries(mc_pmsm_safe.elf optimized ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/GMCLIB/libGMCLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB)

target_link_libraries(mc_pmsm_safe.elf optimized ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.5.1_MCUX/MLIB/libMLIB.a)

link_directories(${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0)

target_link_libraries(mc_pmsm_safe.elf optimized ${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_0/libIEC60730_CM0_Class_B_MCUX_v4_0.a)

TARGET_LINK_LIBRARIES(mc_pmsm_safe.elf -Wl,--end-group)


