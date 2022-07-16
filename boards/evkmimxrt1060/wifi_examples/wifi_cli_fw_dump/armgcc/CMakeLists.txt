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


project(wifi_cli_fw_dump)

set(MCUX_SDK_PROJECT_NAME wifi_cli_fw_dump.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../usb/usb_support.h"
"${ProjDirPath}/../usb/usb_support.c"
"${ProjDirPath}/../usb/usb_host_config.h"
"${ProjDirPath}/../usb/usb_api.h"
"${ProjDirPath}/../usb/host_msd_fatfs.h"
"${ProjDirPath}/../usb/host_msd_fatfs.c"
"${ProjDirPath}/../usb/ffconf.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../usb
    ${ProjDirPath}/../../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../middleware/usb
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../middleware/wifi_nxp
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../components/gpio
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../middleware/fatfs
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../components/silicon_id
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/wifi_bt_module
)

# include modules
include(driver_cache_armv7_m7_MIMXRT1062)

include(middleware_usb_host_stack_MIMXRT1062)

include(component_osa_MIMXRT1062)

include(middleware_usb_host_ehci_MIMXRT1062)

include(middleware_wifi_wifidriver_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(utility_debug_console_MIMXRT1062)

include(middleware_wifi_sdio_MIMXRT1062)

include(middleware_wifi_fwdnld_MIMXRT1062)

include(middleware_sdmmc_sdio_MIMXRT1062)

include(middleware_sdmmc_common_MIMXRT1062)

include(middleware_sdmmc_host_usdhc_MIMXRT1062)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1062)

include(component_igpio_adapter_MIMXRT1062)

include(middleware_freertos-kernel_heap_4_MIMXRT1062)

include(middleware_lwip_MIMXRT1062)

include(middleware_wifi_MIMXRT1062)

include(middleware_lwip_apps_lwiperf_MIMXRT1062)

include(middleware_wifi_cli_MIMXRT1062)

include(middleware_fatfs_MIMXRT1062)

include(middleware_fatfs_usb_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_serial_manager_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_serial_manager_uart_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(utility_assert_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkmimxrt1060_MIMXRT1062)

include(component_silicon_id_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(middleware_usb_phy_MIMXRT1062)

include(middleware_usb_common_header_MIMXRT1062)

include(middleware_usb_host_common_header_MIMXRT1062)

include(middleware_freertos-kernel_extension_MIMXRT1062)

include(middleware_wifi_sdio-2_MIMXRT1062)

include(middleware_sdmmc_osa_freertos_MIMXRT1062)

include(component_osa_free_rtos_MIMXRT1062)

include(driver_usdhc_MIMXRT1062)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT1062)

include(middleware_usb_host_msd_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


