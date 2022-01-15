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
"${ProjDirPath}/../../../../../../../middleware/motor_control/freemaster/pmsm_float_enc.pmp"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../m1_pmsm_appconfig.h"
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
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../../middleware/rtcesl
    ${ProjDirPath}/../../../../../../../middleware/motor_control
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../../xip
    ${ProjDirPath}/../../../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/utilities
)

# include modules
include(driver_dmamux_MIMXRT1176_cm7)

include(driver_xbara_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_adc_etc_MIMXRT1176_cm7)

include(driver_lpadc_MIMXRT1176_cm7)

include(driver_acmp_MIMXRT1176_cm7)

include(middleware_rtcesl_cm7f_MIMXRT1176_cm7)

include(middleware_motor_control_mc_drivers_adcimxrt11xx_pwma_enc_MIMXRT1176_cm7)

include(middleware_motor_control_mcaa_cm7f_MIMXRT1176_cm7)

include(middleware_motor_control_pmsm_float_imcat_MIMXRT1176_cm7)

include(middleware_mc_pmsm_enc_float_MIMXRT1176_cm7)

include(middleware_motor_control_pmsm_float_tsa_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(middleware_fmstr_MIMXRT1176_cm7)

include(middleware_fmstr_platform_gen32le_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/AMCLIB/libAMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GFLIB/libGFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GDFLIB/libGDFLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/GMCLIB/libGMCLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/rtcesl/CM7F_RTCESL_4.7_MCUX/MLIB/libMLIB.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../../middleware/motor_control/pmsm/pmsm_float/mc_algorithms/mcaa_lib/cm7f_mcux/libmcaa_lib.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


