# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.1)

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


project(mc_pmsm_safe)

set(MCUX_SDK_PROJECT_NAME mc_pmsm_safe.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
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
"${ProjDirPath}/../../../../../../MKV11Z7/system_MKV11Z7.c"
"${ProjDirPath}/../../../../../../MKV11Z7/system_MKV11Z7.h"
"${ProjDirPath}/../../../../../../middleware/motor_control/freemaster/pmsm_safe.pmp"
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
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../../devices/MKV11Z7 debug
    ${ProjDirPath}/../mcux/startup
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_drivers
    ${ProjDirPath}/../../../../../../devices/MKV11Z7
    ${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe/mc_state_machine
    ${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_safe
    ${ProjDirPath}/../periph_init
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers
    ${ProjDirPath}/../../../../../../middleware/rtcesl
    ${ProjDirPath}/../../../../../../middleware/safety_iec60730b
    ${ProjDirPath}/../../../../../../middleware/motor_control
    ${ProjDirPath}/../../../../../../devices/MKV11Z7
    ${ProjDirPath}/../../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../../devices/MKV11Z7/utilities
    ${ProjDirPath}/../../../../../../CMSIS/Include
)

# include modules
include(driver_clock_MKV11Z7)

include(middleware_rtcesl_cm0_MKV11Z7)

include(middleware_motor_control-pmsm_safety_MKV11Z7)

include(middleware_motor_control-pmsm_safety_mc_MKV11Z7)

include(driver_common_MKV11Z7)

include(device_MKV11Z7_CMSIS_MKV11Z7)

include(driver_uart_MKV11Z7)

include(middleware_fmstr_MKV11Z7)

include(middleware_fmstr_platform_gen32le_MKV11Z7)

include(utilities_misc_utilities_MKV11Z7)

include(CMSIS_Include_core_cm0plus_MKV11Z7)

include(CMSIS_Include_common_MKV11Z7)

include(CMSIS_Include_dsp_MKV11Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/AMCLIB/libAMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GFLIB/libGFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GDFLIB/libGDFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GMCLIB/libGMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/MLIB/libMLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_1/libIEC60730B_M0_MCUX_v4_1.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_1/libIEC60730B_M0_COM_MCUX_v4_1.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/AMCLIB/libAMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GFLIB/libGFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GDFLIB/libGDFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GMCLIB/libGMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/MLIB/libMLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_1/libIEC60730B_M0_MCUX_v4_1.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/safety_iec60730b/safety/v4_1/libIEC60730B_M0_COM_MCUX_v4_1.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


