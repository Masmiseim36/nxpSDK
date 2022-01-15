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


project(aws_shadow_wifi_nxp)

set(MCUX_SDK_PROJECT_NAME aws_shadow_wifi_nxp.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../main_wifi.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/aws_shadow_config.h"
"${ProjDirPath}/../config_files/core_mqtt_config.h"
"${ProjDirPath}/../config_files/core_pkcs11_config.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/iot_config.h"
"${ProjDirPath}/../config_files/iot_secure_sockets_config.h"
"${ProjDirPath}/../config_files/shadow_config.h"
"${ProjDirPath}/../config_files/shadow_demo_config.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../..
    ${ProjDirPath}/..
    ${ProjDirPath}/../config_files
    ${ProjDirPath}/../../../../src/aws_examples/common
    ${ProjDirPath}/../../../../src/aws_examples/common/lwipopts_wifi_nxp
    ${ProjDirPath}/../../common
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../components/flash/mflash
    ${ProjDirPath}/../../../../../components/flash/mflash/mimxrt1064
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities
    ${ProjDirPath}/../../../../../middleware/wifi
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../components/wifi_bt_module
    ${ProjDirPath}/../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/osa
)

# include modules
include(driver_trng_MIMXRT1064)

include(driver_dcp_MIMXRT1064)

include(driver_cache_armv7_m7_MIMXRT1064)

include(driver_flexspi_MIMXRT1064)

include(component_mflash_file_MIMXRT1064)

include(component_mflash_rt1064_MIMXRT1064)

include(middleware_mbedtls_MIMXRT1064)

include(middleware_mbedtls_port_ksdk_MIMXRT1064)

include(middleware_mbedtls_rt_MIMXRT1064)

include(middleware_freertos-aws_iot_vendor_nxp_wifi_MIMXRT1064)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MIMXRT1064)

include(middleware_freertos-aws_iot_demos_MIMXRT1064)

include(middleware_lwip_MIMXRT1064)

include(utility_debug_console_MIMXRT1064)

include(middleware_wifi_MIMXRT1064)

include(middleware_freertos-aws_iot_demos_shadow_MIMXRT1064)

include(middleware_freertos-aws_iot_demos_dev_mode_key_provisioning_MIMXRT1064)

include(middleware_sdmmc_sdio_MIMXRT1064)

include(middleware_sdmmc_common_MIMXRT1064)

include(middleware_sdmmc_host_usdhc_MIMXRT1064)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_abstractions_transport_MIMXRT1064)

include(middleware_freertos-kernel_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_logging_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1064)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_config_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_mbedtls_MIMXRT1064)

include(middleware_freertos-aws_iot_vendor_nxp_pkcs11_MIMXRT1064)

include(middleware_freertos-kernel_heap_4_MIMXRT1064)

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

include(driver_xip_board_evkmimxrt1064_MIMXRT1064)

include(CMSIS_Include_core_cm_MIMXRT1064)

include(component_mflash_common_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT1064)

include(middleware_freertos-kernel_extension_MIMXRT1064)

include(middleware_wifi_wifidriver_MIMXRT1064)

include(middleware_wifi_fwdnld_MIMXRT1064)

include(middleware_sdmmc_osa_freertos_MIMXRT1064)

include(component_osa_free_rtos_MIMXRT1064)

include(driver_usdhc_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT1064)

include(middleware_freertos-aws_iot_network_manager_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_corejason_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_device_shadow_for_aws_MIMXRT1064)

include(middleware_freertos-aws_iot_mqtt_demo_helpers_MIMXRT1064)

include(middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm_MIMXRT1064)

include(middleware_freertos-aws_iot_pkcs11_helpers_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


