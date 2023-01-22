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


project(wifi_webconfig)

set(MCUX_BUILD_TYPES flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME wifi_webconfig.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../webconfig.c"
"${ProjDirPath}/../webconfig.h"
"${ProjDirPath}/../httpsrv_fs_data.c"
"${ProjDirPath}/../cred_flash_storage.c"
"${ProjDirPath}/../cred_flash_storage.h"
"${ProjDirPath}/../http_server.c"
"${ProjDirPath}/../http_server.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt685
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/devices/MIMXRT685S/drivers
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/middleware/wifi_nxp
    ${SdkRootDirPath}/middleware/edgefast_wifi
    ${SdkRootDirPath}/components/pca9420
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/components/flash/mflash/mimxrt685
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/devices/MIMXRT685S
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/boards/evkmimxrt685/flash_config
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/wifi_bt_module
    ${SdkRootDirPath}/components/osa
)

# include modules
include(middleware_lwip_apps_httpsrv_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(middleware_freertos-kernel_heap_3_MIMXRT685S_cm33)

include(middleware_lwip_MIMXRT685S_cm33)

include(middleware_wifi_MIMXRT685S_cm33)

include(middleware_edgefast_wifi_nxp_MIMXRT685S_cm33)

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_pca9420_MIMXRT685S_cm33)

include(component_mflash_file_MIMXRT685S_cm33)

include(component_mflash_rt685_MIMXRT685S_cm33)

include(middleware_wifi_wifidriver_MIMXRT685S_cm33)

include(middleware_freertos-kernel_MIMXRT685S_cm33)

include(utility_debug_console_MIMXRT685S_cm33)

include(middleware_wifi_sdio_MIMXRT685S_cm33)

include(middleware_wifi_fwdnld_MIMXRT685S_cm33)

include(middleware_sdmmc_sdio_MIMXRT685S_cm33)

include(middleware_sdmmc_common_MIMXRT685S_cm33)

include(middleware_sdmmc_host_usdhc_MIMXRT685S_cm33)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT685S_cm33)

include(component_rt_gpio_adapter_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(utility_assert_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_flash_config_evkmimxrt685_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(component_serial_manager_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_serial_manager_uart_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT685S_cm33)

include(middleware_edgefast_wifi_MIMXRT685S_cm33)

include(component_mflash_common_MIMXRT685S_cm33)

include(middleware_wifi_sdio-2_MIMXRT685S_cm33)

include(middleware_sdmmc_osa_freertos_MIMXRT685S_cm33)

include(component_osa_free_rtos_MIMXRT685S_cm33)

include(driver_usdhc_MIMXRT685S_cm33)

include(middleware_wifi_common_files_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

