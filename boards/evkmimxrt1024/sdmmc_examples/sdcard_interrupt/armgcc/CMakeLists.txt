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


project(sdcard_interrupt)

set(MCUX_SDK_PROJECT_NAME sdcard_interrupt.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../sdcard_interrupt.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
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
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/drivers
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../devices/MIMXRT1024
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/utilities
)

# include modules
include(driver_cache_armv7_m7_MIMXRT1024)

include(driver_usdhc_MIMXRT1024)

include(middleware_sdmmc_sd_MIMXRT1024)

include(middleware_sdmmc_common_MIMXRT1024)

include(middleware_sdmmc_host_usdhc_MIMXRT1024)

include(middleware_sdmmc_host_usdhc_interrupt_MIMXRT1024)

include(utility_debug_console_lite_MIMXRT1024)

include(utility_assert_lite_MIMXRT1024)

include(driver_clock_MIMXRT1024)

include(driver_common_MIMXRT1024)

include(device_MIMXRT1024_CMSIS_MIMXRT1024)

include(component_lpuart_adapter_MIMXRT1024)

include(component_lists_MIMXRT1024)

include(driver_lpuart_MIMXRT1024)

include(device_MIMXRT1024_startup_MIMXRT1024)

include(driver_iomuxc_MIMXRT1024)

include(driver_igpio_MIMXRT1024)

include(driver_xip_device_MIMXRT1024)

include(driver_xip_board_MIMXRT1024)

include(CMSIS_Include_core_cm7_MIMXRT1024)

include(CMSIS_Include_common_MIMXRT1024)

include(CMSIS_Include_dsp_MIMXRT1024)

include(middleware_sdmmc_osa_bm_MIMXRT1024)

include(component_osa_bm_MIMXRT1024)

include(utilities_misc_utilities_MIMXRT1024)

include(device_MIMXRT1024_system_MIMXRT1024)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


