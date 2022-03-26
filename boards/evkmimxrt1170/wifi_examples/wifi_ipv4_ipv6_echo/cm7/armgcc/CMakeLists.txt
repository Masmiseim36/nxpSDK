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


project(wifi_ipv4_ipv6_echo_cm7)

set(MCUX_SDK_PROJECT_NAME wifi_ipv4_ipv6_echo_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../shell_task.c"
"${ProjDirPath}/../shell_task.h"
"${ProjDirPath}/../shell_task_mode.h"
"${ProjDirPath}/../socket_task.c"
"${ProjDirPath}/../socket_task.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm7side.jlinkscript"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../common/cm7
    ${ProjDirPath}/../../../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/utilities
    ${ProjDirPath}/../../../../../../middleware/wifi_nxp
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers/cm7
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../components/wifi_bt_module
    ${ProjDirPath}/../../../../../../components/osa
)

# include modules
include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_lwip_MIMXRT1176_cm7)

include(utility_debug_console_MIMXRT1176_cm7)

include(middleware_wifi_MIMXRT1176_cm7)

include(driver_usdhc_MIMXRT1176_cm7)

include(middleware_sdmmc_sdio_MIMXRT1176_cm7)

include(middleware_sdmmc_common_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1176_cm7)

include(utility_shell_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_serial_manager_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_serial_manager_uart_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(utility_assert_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_cache_armv7_m7_MIMXRT1176_cm7)

include(middleware_freertos-kernel_extension_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(middleware_wifi_wifidriver_MIMXRT1176_cm7)

include(middleware_wifi_fwdnld_MIMXRT1176_cm7)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT1176_cm7)

include(middleware_sdmmc_osa_freertos_MIMXRT1176_cm7)

include(component_osa_free_rtos_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


