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
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../remote_control.c"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
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
    ${ProjDirPath}/..
    ${ProjDirPath}/../config_files
    ${ProjDirPath}/../../..
)

if(CMAKE_BUILD_TYPE STREQUAL flash_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_debug)

if(CMAKE_BUILD_TYPE STREQUAL flash_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/utilities
    ${ProjDirPath}/../../../../../middleware/wifi
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/drivers
    ${ProjDirPath}/../../../../../components/flash/mflash
    ${ProjDirPath}/../../../../../components/flash/mflash/mimxrt595
    ${ProjDirPath}/../../../../../components/pca9420
    ${ProjDirPath}/../../../../../components/fxos8700cq
    ${ProjDirPath}/../../../../../devices/MIMXRT595S
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../flash_config
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/osa
)

# include modules
include(middleware_freertos-aws_iot_vendor_nxp_wifi_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_demos_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_demos_dev_mode_key_provisioning_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MIMXRT595S_cm33)

include(middleware_lwip_MIMXRT595S_cm33)

include(utility_debug_console_MIMXRT595S_cm33)

include(middleware_wifi_MIMXRT595S_cm33)

include(middleware_sdmmc_sdio_MIMXRT595S_cm33)

include(middleware_sdmmc_common_MIMXRT595S_cm33)

include(middleware_sdmmc_host_usdhc_MIMXRT595S_cm33)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_c_sdk_standard_shadow_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_c_sdk_standard_serializer_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_vendor_nxp_pkcs11_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT595S_cm33)

include(middleware_mbedtls_MIMXRT595S_cm33)

include(middleware_mbedtls_port_ksdk_MIMXRT595S_cm33)

include(driver_casper_MIMXRT595S_cm33)

include(driver_hashcrypt_MIMXRT595S_cm33)

include(driver_trng_MIMXRT595S_cm33)

include(component_mflash_file_MIMXRT595S_cm33)

include(component_mflash_rt595_MIMXRT595S_cm33)

include(middleware_mbedtls_rt1_MIMXRT595S_cm33)

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT595S_cm33)

include(driver_flexcomm_i2c_MIMXRT595S_cm33)

include(driver_pca9420_MIMXRT595S_cm33)

include(driver_fxos8700cq_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(utility_assert_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(component_serial_manager_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(component_serial_manager_uart_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(driver_flash_config_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_common_MIMXRT595S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm33_MIMXRT595S_cm33)

include(CMSIS_Include_common_MIMXRT595S_cm33)

include(CMSIS_Include_dsp_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_threading_alt_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_network_manager_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_mqtt_MIMXRT595S_cm33)

include(middleware_sdmmc_osa_freertos_MIMXRT595S_cm33)

include(component_osa_free_rtos_MIMXRT595S_cm33)

include(driver_usdhc_MIMXRT595S_cm33)

include(component_mflash_common_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

