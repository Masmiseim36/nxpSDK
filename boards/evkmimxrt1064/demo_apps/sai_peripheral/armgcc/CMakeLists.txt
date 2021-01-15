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


project(sai_peripheral)

set(MCUX_SDK_PROJECT_NAME sai_peripheral.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../recordtoSdcard.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sai.h"
"${ProjDirPath}/../sai_peripheral.c"
"${ProjDirPath}/../recordPlayback.c"
"${ProjDirPath}/../playbackSineWave.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../peripherals.c"
"${ProjDirPath}/../peripherals.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../components/codec/wm8960
    ${ProjDirPath}/../../../../../CMSIS/DSP/Lib
    ${ProjDirPath}/../../../../../middleware/fatfs
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../components/codec/port/wm8960
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities
)

# include modules
include(driver_lpi2c_MIMXRT1064)

include(driver_wm8960_MIMXRT1064)

include(CMSIS_DSP_Lib_arm_cortexM7lfsp_math_MIMXRT1064)

include(driver_usdhc_MIMXRT1064)

include(middleware_fatfs_MIMXRT1064)

include(middleware_fatfs_sd_MIMXRT1064)

include(driver_cache_armv7_m7_MIMXRT1064)

include(middleware_sdmmc_sd_MIMXRT1064)

include(middleware_sdmmc_common_MIMXRT1064)

include(middleware_sdmmc_host_usdhc_MIMXRT1064)

include(middleware_sdmmc_host_usdhc_interrupt_MIMXRT1064)

include(driver_codec_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(component_wm8960_adapter_MIMXRT1064)

include(component_codec_i2c_MIMXRT1064)

include(component_lpi2c_adapter_MIMXRT1064)

include(utility_debug_console_lite_MIMXRT1064)

include(utility_assert_lite_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_sai_MIMXRT1064)

include(driver_sai_edma_MIMXRT1064)

include(driver_dmamux_MIMXRT1064)

include(driver_edma_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_MIMXRT1064)

include(CMSIS_Include_core_cm7_MIMXRT1064)

include(CMSIS_Include_common_MIMXRT1064)

include(CMSIS_Include_dsp_MIMXRT1064)

include(middleware_sdmmc_osa_bm_MIMXRT1064)

include(component_osa_bm_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfsp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfsp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfsp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfsp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfsp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfsp_math.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


