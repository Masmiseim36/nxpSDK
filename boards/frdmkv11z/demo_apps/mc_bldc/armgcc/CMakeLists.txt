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


project(mc_bldc)

set(MCUX_SDK_PROJECT_NAME mc_bldc.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/motor_control/bldc/mc_drivers/mcdrv_adc_adc16.h"
"${ProjDirPath}/../../../../../middleware/motor_control/bldc/mc_drivers/mcdrv_adc_adc16.c"
"${ProjDirPath}/../../../../../middleware/motor_control/bldc/mc_drivers/mcdrv_ftm_cmt.c"
"${ProjDirPath}/../../../../../middleware/motor_control/bldc/mc_drivers/mcdrv_ftm_cmt.h"
"${ProjDirPath}/../../../../../middleware/motor_control/bldc/mc_drivers/mcdrv_pwm3ph_ftm.c"
"${ProjDirPath}/../../../../../middleware/motor_control/bldc/mc_drivers/mcdrv_pwm3ph_ftm.h"
"${ProjDirPath}/../../../../../middleware/motor_control/freemaster/bldc.pmp"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../main.h"
"${ProjDirPath}/../m1_bldc_appconfig.h"
"${ProjDirPath}/../mcdrv_frdmkv11z.c"
"${ProjDirPath}/../mcdrv_frdmkv11z.h"
"${ProjDirPath}/../mcdrv.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../peripherals.c"
"${ProjDirPath}/../peripherals.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../middleware/motor_control/bldc/mc_drivers
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MKV11Z7/drivers
    ${ProjDirPath}/../../../../../middleware/rtcesl
    ${ProjDirPath}/../../../../../middleware/motor_control
    ${ProjDirPath}/../../../../../devices/MKV11Z7
    ${ProjDirPath}/../../../../../devices/MKV11Z7/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(driver_sim_MKV11Z7)

include(driver_port_MKV11Z7)

include(driver_dmamux_MKV11Z7)

include(driver_clock_MKV11Z7)

include(middleware_rtcesl_cm0_MKV11Z7)

include(middleware_motor_control-bldc_MKV11Z7)

include(driver_common_MKV11Z7)

include(device_MKV11Z7_CMSIS_MKV11Z7)

include(utility_debug_console_MKV11Z7)

include(component_uart_adapter_MKV11Z7)

include(component_serial_manager_MKV11Z7)

include(component_lists_MKV11Z7)

include(component_serial_manager_uart_MKV11Z7)

include(driver_uart_MKV11Z7)

include(device_MKV11Z7_startup_MKV11Z7)

include(driver_smc_MKV11Z7)

include(driver_gpio_MKV11Z7)

include(utility_assert_MKV11Z7)

include(middleware_fmstr_MKV11Z7)

include(middleware_fmstr_platform_gen32le_MKV11Z7)

include(CMSIS_Include_core_cm0plus_MKV11Z7)

include(CMSIS_Include_common_MKV11Z7)

include(CMSIS_Include_dsp_MKV11Z7)

include(utilities_misc_utilities_MKV11Z7)

include(device_MKV11Z7_system_MKV11Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/AMCLIB/libAMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GFLIB/libGFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GDFLIB/libGDFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GMCLIB/libGMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/MLIB/libMLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/AMCLIB/libAMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GFLIB/libGFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GDFLIB/libGDFLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/GMCLIB/libGMCLIB.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/rtcesl/CM0_RTCESL_4.6_MCUX/MLIB/libMLIB.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


