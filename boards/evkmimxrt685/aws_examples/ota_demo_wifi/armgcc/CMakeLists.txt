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


project(ota_demo_wifi)

set(MCUX_BUILD_TYPES debug release flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME ota_demo_wifi.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../config_files/core_http_config.h"
"${ProjDirPath}/../config_files/core_mqtt_config.h"
"${ProjDirPath}/../config_files/core_pkcs11_config.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/iot_config.h"
"${ProjDirPath}/../config_files/iot_secure_sockets_config.h"
"${ProjDirPath}/../config_files/ota_config.h"
"${ProjDirPath}/../config_files/ota_demo_config.h"
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../main_wifi.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../mcuboot_app_support.c"
"${ProjDirPath}/../mcuboot_app_support.h"
"${ProjDirPath}/../bootutil_port/board_specific/flash_partitioning.h"
"${ProjDirPath}/../bootutil_port/board_specific/flash_partitioning.c"
"${ProjDirPath}/../bootutil_port/board_specific/sblconfig.h"
"${ProjDirPath}/../bootutil_port/mcuboot_config/mcuboot_config.h"
"${ProjDirPath}/../bootutil_port/mcuboot_config/mcuboot_logging.h"
"${ProjDirPath}/../bootutil_port/flash_map.h"
"${ProjDirPath}/../bootutil_port/sysflash/sysflash.h"
"${ProjDirPath}/../bootutil_port/sbldef.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../config_files
    ${SdkRootDirPath}/boards/evkmimxrt685
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt685/aws_examples/common
    ${ProjDirPath}/../bootutil_port
    ${ProjDirPath}/../bootutil_port/mcuboot_config
    ${ProjDirPath}/../bootutil_port/sysflash
    ${ProjDirPath}/../bootutil_port/board_specific
)

if(CMAKE_BUILD_TYPE STREQUAL debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL flash_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_debug)

if(CMAKE_BUILD_TYPE STREQUAL flash_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/devices/MIMXRT685S/drivers
    ${SdkRootDirPath}/components/pca9420
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/components/flash/mflash/mimxrt685
    ${SdkRootDirPath}/middleware/wifi_nxp
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/middleware/aws_iot/amazon-freertos
    ${SdkRootDirPath}/middleware/aws_iot/ota
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/rtos/freertos/coremqtt
    ${SdkRootDirPath}/components/wifi_bt_module
    ${SdkRootDirPath}/middleware/pkcs11
    ${SdkRootDirPath}/rtos/freertos/corepkcs11
    ${SdkRootDirPath}/devices/MIMXRT685S
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/rtos/freertos/corejson
    ${SdkRootDirPath}/middleware/tinycbor
    ${SdkRootDirPath}/rtos/freertos/backoffalgorithm
    ${SdkRootDirPath}/rtos/freertos/coremqtt-agent
)

# include modules
include(middleware_mbedtls_MIMXRT685S_cm33)

include(middleware_mbedtls_port_ksdk_MIMXRT685S_cm33)

include(middleware_mbedtls_rt1_MIMXRT685S_cm33)

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

include(middleware_amazon-freertos_demos_MIMXRT685S_cm33)

include(middleware_amazon-freertos_demos_dev_mode_key_provisioning_MIMXRT685S_cm33)

include(middleware_amazon-freertos_demos_ota_mqtt_MIMXRT685S_cm33)

include(middleware_aws_iot_ota_MIMXRT685S_cm33)

include(middleware_amazon-freertos_vendor_nxp_secure_sockets_lwip_MIMXRT685S_cm33)

include(middleware_amazon-freertos_vendor_nxp_wifi_MIMXRT685S_cm33)

include(middleware_amazon-freertos_vendor_nxp_ota_pal_MIMXRT685S_cm33)

include(middleware_lwip_MIMXRT685S_cm33)

include(middleware_wifi_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_transport_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_secure_sockets_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_logging_MIMXRT685S_cm33)

include(middleware_freertos_coremqtt_MIMXRT685S_cm33)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT685S_cm33)

include(middleware_pkcs11_MIMXRT685S_cm33)

include(middleware_freertos_corepkcs11_MIMXRT685S_cm33)

include(middleware_freertos_corepkcs11_mbedtls_MIMXRT685S_cm33)

include(middleware_amazon-freertos_vendor_nxp_pkcs11_MIMXRT685S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(utility_assert_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

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

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(driver_casper_MIMXRT685S_cm33)

include(driver_hashcrypt_MIMXRT685S_cm33)

include(driver_trng_MIMXRT685S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT685S_cm33)

include(component_mflash_common_MIMXRT685S_cm33)

include(middleware_wifi_sdio-2_MIMXRT685S_cm33)

include(middleware_sdmmc_osa_freertos_MIMXRT685S_cm33)

include(component_osa_free_rtos_MIMXRT685S_cm33)

include(driver_usdhc_MIMXRT685S_cm33)

include(middleware_wifi_common_files_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_platform_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_include_MIMXRT685S_cm33)

include(middleware_amazon-freertos_network_manager_MIMXRT685S_cm33)

include(middleware_amazon-freertos_demos_ota_demo_helpers_MIMXRT685S_cm33)

include(middleware_freertos_corejson_MIMXRT685S_cm33)

include(middleware_tinycbor_MIMXRT685S_cm33)

include(middleware_freertos_backoffalgorithm_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_mqtt_agent_MIMXRT685S_cm33)

include(middleware_freertos_coremqtt-agent_MIMXRT685S_cm33)

include(middleware_aws_iot_ota_freertos_MIMXRT685S_cm33)

include(middleware_amazon-freertos_pkcs11_helpers_MIMXRT685S_cm33)

include(middleware_amazon-freertos_demos_common_mqtt_subscription_manager_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_tls_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_3rdparty_mbedtls_utils_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_utils_MIMXRT685S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_wifi_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/ota_demo.bin)

