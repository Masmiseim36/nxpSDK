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


project(aws_remote_control_wifi_nxp)

set(MCUX_SDK_PROJECT_NAME aws_remote_control_wifi_nxp.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../remote_control.c"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../config_files/aws_bufferpool_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/aws_mqtt_config.h"
"${ProjDirPath}/../config_files/aws_secure_sockets_config.h"
"${ProjDirPath}/../config_files/aws_shadow_config.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/iot_config.h"
"${ProjDirPath}/../config_files/iot_mqtt_agent_config.h"
"${ProjDirPath}/../config_files/iot_pkcs11_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../config_files flash_debug
    ${ProjDirPath}/../../.. flash_debug
    ${ProjDirPath}/../config_files
    ${ProjDirPath}/..
    ${ProjDirPath}/../../..
)

if(CMAKE_BUILD_TYPE STREQUAL flash_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_debug)

if(CMAKE_BUILD_TYPE STREQUAL flash_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../components/flash/mflash
    ${ProjDirPath}/../../../../../components/flash/mflash/mimxrt685
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../components/pca9420
    ${ProjDirPath}/../../../../../components/fxos8700cq
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities
    ${ProjDirPath}/../../../../../middleware/wifi
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../devices/MIMXRT685S
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../flash_config
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/osa
)

# include modules
include(driver_casper_MIMXRT685S_cm33)

include(driver_hashcrypt_MIMXRT685S_cm33)

include(driver_trng_MIMXRT685S_cm33)

include(middleware_mbedtls_port_ksdk_MIMXRT685S_cm33)

include(middleware_mbedtls_rt1_MIMXRT685S_cm33)

include(component_mflash_file_MIMXRT685S_cm33)

include(component_mflash_rt685_MIMXRT685S_cm33)

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_pca9420_MIMXRT685S_cm33)

include(driver_fxos8700cq_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_vendor_nxp_wifi_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_demos_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_demos_dev_mode_key_provisioning_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MIMXRT685S_cm33)

include(middleware_lwip_MIMXRT685S_cm33)

include(utility_debug_console_MIMXRT685S_cm33)

include(middleware_wifi_MIMXRT685S_cm33)

include(middleware_sdmmc_sdio_MIMXRT685S_cm33)

include(middleware_sdmmc_common_MIMXRT685S_cm33)

include(middleware_sdmmc_host_usdhc_MIMXRT685S_cm33)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT685S_cm33)

include(middleware_freertos-kernel_MIMXRT685S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_c_sdk_standard_shadow_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_c_sdk_standard_serializer_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_vendor_nxp_pkcs11_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT685S_cm33)

include(middleware_mbedtls_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(utility_assert_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_flash_config_MIMXRT685S_cm33)

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

include(CMSIS_Include_core_cm33_MIMXRT685S_cm33)

include(CMSIS_Include_common_MIMXRT685S_cm33)

include(CMSIS_Include_dsp_MIMXRT685S_cm33)

include(component_mflash_common_MIMXRT685S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_common_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_threading_alt_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_network_manager_MIMXRT685S_cm33)

include(middleware_freertos-aws_iot_mqtt_MIMXRT685S_cm33)

include(middleware_sdmmc_osa_freertos_MIMXRT685S_cm33)

include(component_osa_free_rtos_MIMXRT685S_cm33)

include(driver_usdhc_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

