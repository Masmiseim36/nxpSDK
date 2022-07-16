# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

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


project(mc_pmsm)

set(MCUX_SDK_PROJECT_NAME mc_pmsm.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../../middleware/motor_control/freemaster/pmsm_float.pmp"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../m1_pmsm_appconfig.h"
"${ProjDirPath}/../m2_pmsm_appconfig.h"
"${ProjDirPath}/../mc_periph_init.c"
"${ProjDirPath}/../char_pwrstg.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../peripherals.h"
"${ProjDirPath}/../peripherals.c"
"${ProjDirPath}/../freemaster_cfg.h"
"${ProjDirPath}/../mc_periph_init.h"
"${ProjDirPath}/../readme.txt"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT1011/drivers
    ${ProjDirPath}/../../../../../../middleware/rtcesl
    ${ProjDirPath}/../../../../../../middleware/motor_control
    ${ProjDirPath}/../../../../../../devices/MIMXRT1011
    ${ProjDirPath}/../../../../../../devices/MIMXRT1011/utilities
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../devices/MIMXRT1011/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
)

# include modules
include(driver_dmamux_MIMXRT1011)

include(driver_xbara_MIMXRT1011)

include(driver_clock_MIMXRT1011)

include(driver_adc_etc_MIMXRT1011)

include(middleware_rtcesl_cm7f_MIMXRT1011)

include(middleware_motor_control_mc_drivers_adcimxrt1010_pwma_MIMXRT1011)

include(middleware_motor_control_mcaa_cm7f_MIMXRT1011)

include(middleware_motor_control_pmsm_float_imcat_MIMXRT1011)

include(middleware_mc_pmsm_snsless_float_MIMXRT1011)

include(middleware_motor_control_pmsm_float_tsa_MIMXRT1011)

include(driver_common_MIMXRT1011)

include(device_MIMXRT1011_CMSIS_MIMXRT1011)

include(utility_debug_console_MIMXRT1011)

include(component_lpuart_adapter_MIMXRT1011)

include(component_serial_manager_MIMXRT1011)

include(component_lists_MIMXRT1011)

include(component_serial_manager_uart_MIMXRT1011)

include(driver_lpuart_MIMXRT1011)

include(device_MIMXRT1011_startup_MIMXRT1011)

include(driver_pit_MIMXRT1011)

include(driver_iomuxc_MIMXRT1011)

include(utility_assert_MIMXRT1011)

include(driver_igpio_MIMXRT1011)

include(driver_xip_device_MIMXRT1011)

include(driver_xip_board_evkmimxrt1010_MIMXRT1011)

include(middleware_fmstr_MIMXRT1011)

include(middleware_fmstr_platform_gen32le_MIMXRT1011)

include(CMSIS_Include_core_cm_MIMXRT1011)

include(utilities_misc_utilities_MIMXRT1011)

include(device_MIMXRT1011_system_MIMXRT1011)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7.1_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


