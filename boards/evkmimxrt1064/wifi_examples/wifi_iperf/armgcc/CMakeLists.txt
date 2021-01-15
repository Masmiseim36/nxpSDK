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


project(wifi_iperf)

set(MCUX_SDK_PROJECT_NAME wifi_iperf.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../sdmmc_config.c"
"${ProjDirPath}/../../../sdmmc_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. flexspi_nor_debug
    ${ProjDirPath}/.. flexspi_nor_debug
    ${ProjDirPath}/..
    ${ProjDirPath}/../../common
    ${ProjDirPath}/../../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities
    ${ProjDirPath}/../../../../../middleware/wifi
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/osa
)

# include modules
include(driver_cache_armv7_m7_MIMXRT1064)

include(middleware_freertos-kernel_MIMXRT1064)

include(middleware_freertos-kernel_heap_4_MIMXRT1064)

include(middleware_lwip_MIMXRT1064)

include(utility_debug_console_MIMXRT1064)

include(middleware_wifi_MIMXRT1064)

include(middleware_lwip_apps_lwiperf_MIMXRT1064)

include(driver_usdhc_MIMXRT1064)

include(middleware_sdmmc_sdio_MIMXRT1064)

include(middleware_sdmmc_common_MIMXRT1064)

include(middleware_sdmmc_host_usdhc_MIMXRT1064)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(component_serial_manager_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(component_serial_manager_uart_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(utility_assert_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_MIMXRT1064)

include(CMSIS_Include_core_cm7_MIMXRT1064)

include(CMSIS_Include_common_MIMXRT1064)

include(CMSIS_Include_dsp_MIMXRT1064)

include(middleware_freertos-kernel_extension_MIMXRT1064)

include(middleware_sdmmc_osa_freertos_MIMXRT1064)

include(component_osa_free_rtos_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


