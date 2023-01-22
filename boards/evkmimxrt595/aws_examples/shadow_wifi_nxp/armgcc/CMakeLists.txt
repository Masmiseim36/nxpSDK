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

set(MCUX_BUILD_TYPES flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME aws_shadow_wifi_nxp.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.c"
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../main_wifi.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../core_mqtt_config.h"
"${ProjDirPath}/../core_pkcs11_config.h"
"${ProjDirPath}/../demo_config.h"
"${ProjDirPath}/../freertos_tasks_c_additions.h"
"${ProjDirPath}/../kvstore_config.h"
"${ProjDirPath}/../shadow_config.h"
"${ProjDirPath}/../aws_clientcredential.h"
"${ProjDirPath}/../aws_clientcredential_keys.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../app_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../config_files
    ${SdkRootDirPath}/boards/evkmimxrt595
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt595/aws_examples/common
)

if(CMAKE_BUILD_TYPE STREQUAL flash_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_debug)

if(CMAKE_BUILD_TYPE STREQUAL flash_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flash_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/middleware/wifi_nxp
    ${SdkRootDirPath}/components/wifi_bt_module
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/kvstore
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/logging
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/mqtt_agent
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/Middleware/FreeRTOS/mqtt-agent-interface
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/evkbmimxrt1060/shadow
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/Middleware/FreeRTOS/transport_mbedtls
    ${SdkRootDirPath}/rtos/freertos/coremqtt
    ${SdkRootDirPath}/rtos/freertos/coremqtt-agent
    ${SdkRootDirPath}/rtos/freertos/corepkcs11
    ${SdkRootDirPath}/middleware/edgefast_wifi
    ${SdkRootDirPath}/middleware/pkcs11
    ${SdkRootDirPath}/middleware/aws_iot/amazon-freertos
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/devices/MIMXRT595S/drivers
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/components/flash/mflash/mimxrt595
    ${SdkRootDirPath}/components/pca9420
    ${SdkRootDirPath}/devices/MIMXRT595S/utilities
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/devices/MIMXRT595S
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/boards/evkmimxrt595/flash_config
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/rtos/freertos/backoffalgorithm
    ${SdkRootDirPath}/middleware/aws_iot/device-shadow
    ${SdkRootDirPath}/rtos/freertos/corejson
    ${SdkRootDirPath}/components/osa
)

# include modules
include(middleware_lwip_MIMXRT595S_cm33)

include(middleware_wifi_MIMXRT595S_cm33)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT595S_cm33)

include(middleware_iot_reference_kvstore_MIMXRT595S_cm33)

include(middleware_iot_reference_logging_MIMXRT595S_cm33)

include(middleware_iot_reference_mqtt_agent_MIMXRT595S_cm33)

include(middleware_iot_reference_mqtt_agent_interface_MIMXRT595S_cm33)

include(middleware_iot_reference_shadow_tasks_MIMXRT595S_cm33)

include(middleware_iot_reference_transport_mbedtls_MIMXRT595S_cm33)

include(middleware_freertos_coremqtt_MIMXRT595S_cm33)

include(middleware_freertos_coremqtt-agent_MIMXRT595S_cm33)

include(middleware_freertos_corepkcs11_mbedtls_utils_MIMXRT595S_cm33)

include(middleware_edgefast_wifi_nxp_MIMXRT595S_cm33)

include(middleware_pkcs11_MIMXRT595S_cm33)

include(middleware_freertos_corepkcs11_MIMXRT595S_cm33)

include(middleware_freertos_corepkcs11_mbedtls_MIMXRT595S_cm33)

include(middleware_amazon-freertos_vendor_nxp_pkcs11_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT595S_cm33)

include(middleware_mbedtls_MIMXRT595S_cm33)

include(driver_casper_MIMXRT595S_cm33)

include(driver_hashcrypt_MIMXRT595S_cm33)

include(driver_trng_MIMXRT595S_cm33)

include(component_mflash_file_MIMXRT595S_cm33)

include(component_mflash_rt595_MIMXRT595S_cm33)

include(middleware_mbedtls_port_ksdk_MIMXRT595S_cm33)

include(middleware_mbedtls_rt1_MIMXRT595S_cm33)

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT595S_cm33)

include(driver_flexcomm_i2c_MIMXRT595S_cm33)

include(driver_pca9420_MIMXRT595S_cm33)

include(middleware_wifi_wifidriver_MIMXRT595S_cm33)

include(utility_debug_console_MIMXRT595S_cm33)

include(middleware_wifi_sdio_MIMXRT595S_cm33)

include(middleware_wifi_fwdnld_MIMXRT595S_cm33)

include(middleware_sdmmc_sdio_MIMXRT595S_cm33)

include(middleware_sdmmc_common_MIMXRT595S_cm33)

include(middleware_sdmmc_host_usdhc_MIMXRT595S_cm33)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT595S_cm33)

include(component_rt_gpio_adapter_MIMXRT595S_cm33)

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

include(driver_flash_config_evkmimxrt595_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(middleware_freertos_backoffalgorithm_MIMXRT595S_cm33)

include(middleware_aws_iot_device_shadow_MIMXRT595S_cm33)

include(middleware_freertos_corejson_MIMXRT595S_cm33)

include(middleware_edgefast_wifi_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT595S_cm33)

include(component_mflash_common_MIMXRT595S_cm33)

include(middleware_wifi_sdio-2_MIMXRT595S_cm33)

include(middleware_sdmmc_osa_freertos_MIMXRT595S_cm33)

include(component_osa_free_rtos_MIMXRT595S_cm33)

include(driver_usdhc_MIMXRT595S_cm33)

include(middleware_wifi_common_files_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

