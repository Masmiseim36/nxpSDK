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


project(wifi_test_mode)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME wifi_test_mode.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkbimxrt1050/wifi_examples/common
    ${SdkRootDirPath}/boards/evkbimxrt1050
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1052/drivers
    ${SdkRootDirPath}/middleware/wifi_nxp
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/devices/MIMXRT1052/utilities
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/devices/MIMXRT1052
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1052/xip
    ${SdkRootDirPath}/boards/evkbimxrt1050/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/components/wifi_bt_module
)

# include modules
include(driver_cache_armv7_m7_MIMXRT1052)

include(middleware_wifi_wifidriver_MIMXRT1052)

include(middleware_freertos-kernel_MIMXRT1052)

include(utility_debug_console_MIMXRT1052)

include(middleware_wifi_sdio_MIMXRT1052)

include(middleware_wifi_fwdnld_MIMXRT1052)

include(middleware_sdmmc_sdio_MIMXRT1052)

include(middleware_sdmmc_common_MIMXRT1052)

include(middleware_sdmmc_host_usdhc_MIMXRT1052)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1052)

include(component_igpio_adapter_MIMXRT1052)

include(middleware_freertos-kernel_heap_4_MIMXRT1052)

include(middleware_lwip_MIMXRT1052)

include(middleware_wifi_MIMXRT1052)

include(middleware_lwip_apps_lwiperf_MIMXRT1052)

include(middleware_wifi_cli_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(component_serial_manager_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(component_serial_manager_uart_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(utility_assert_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_evkbimxrt1050_MIMXRT1052)

include(CMSIS_Include_core_cm_MIMXRT1052)

include(middleware_freertos-kernel_extension_MIMXRT1052)

include(middleware_wifi_sdio-2_MIMXRT1052)

include(middleware_sdmmc_osa_freertos_MIMXRT1052)

include(component_osa_free_rtos_MIMXRT1052)

include(driver_usdhc_MIMXRT1052)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT1052)

include(middleware_wifi_common_files_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


