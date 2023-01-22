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


project(aws_shadow_wifi_nxp_cm7)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME aws_shadow_wifi_nxp_cm7.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../main_wifi.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.c"
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
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm7side.jlinkscript"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../config_files
    ${SdkRootDirPath}/boards/evkmimxrt1170
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt1170/aws_examples/common/cm7
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1176/drivers
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/components/flash/mflash/mimxrt1170
    ${SdkRootDirPath}/devices/MIMXRT1176/drivers/cm7
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/middleware/wifi_nxp
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/devices/MIMXRT1176/utilities
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/middleware/lwip
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
    ${SdkRootDirPath}/devices/MIMXRT1176
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1176/xip
    ${SdkRootDirPath}/boards/evkmimxrt1170/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/rtos/freertos/backoffalgorithm
    ${SdkRootDirPath}/middleware/aws_iot/device-shadow
    ${SdkRootDirPath}/rtos/freertos/corejson
)

# include modules
include(driver_caam_MIMXRT1176_cm7)

include(driver_flexspi_MIMXRT1176_cm7)

include(component_mflash_file_MIMXRT1176_cm7)

include(component_mflash_rt1170_MIMXRT1176_cm7)

include(component_mflash_common_MIMXRT1176_cm7)

include(driver_cache_armv7_m7_MIMXRT1176_cm7)

include(middleware_mbedtls_MIMXRT1176_cm7)

include(middleware_mbedtls_port_ksdk_MIMXRT1176_cm7)

include(middleware_mbedtls_rt2_MIMXRT1176_cm7)

include(middleware_wifi_wifidriver_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(utility_debug_console_MIMXRT1176_cm7)

include(middleware_wifi_sdio_MIMXRT1176_cm7)

include(middleware_wifi_fwdnld_MIMXRT1176_cm7)

include(middleware_sdmmc_sdio_MIMXRT1176_cm7)

include(middleware_sdmmc_common_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1176_cm7)

include(component_igpio_adapter_MIMXRT1176_cm7)

include(middleware_lwip_MIMXRT1176_cm7)

include(middleware_wifi_MIMXRT1176_cm7)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT1176_cm7)

include(middleware_iot_reference_kvstore_MIMXRT1176_cm7)

include(middleware_iot_reference_logging_MIMXRT1176_cm7)

include(middleware_iot_reference_mqtt_agent_MIMXRT1176_cm7)

include(middleware_iot_reference_mqtt_agent_interface_MIMXRT1176_cm7)

include(middleware_iot_reference_shadow_tasks_MIMXRT1176_cm7)

include(middleware_iot_reference_transport_mbedtls_MIMXRT1176_cm7)

include(middleware_freertos_coremqtt_MIMXRT1176_cm7)

include(middleware_freertos_coremqtt-agent_MIMXRT1176_cm7)

include(middleware_freertos_corepkcs11_mbedtls_utils_MIMXRT1176_cm7)

include(middleware_edgefast_wifi_nxp_MIMXRT1176_cm7)

include(middleware_pkcs11_MIMXRT1176_cm7)

include(middleware_freertos_corepkcs11_MIMXRT1176_cm7)

include(middleware_freertos_corepkcs11_mbedtls_MIMXRT1176_cm7)

include(middleware_amazon-freertos_vendor_nxp_pkcs11_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

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

include(driver_memory_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(middleware_freertos-kernel_extension_MIMXRT1176_cm7)

include(middleware_wifi_sdio-2_MIMXRT1176_cm7)

include(middleware_sdmmc_osa_freertos_MIMXRT1176_cm7)

include(component_osa_free_rtos_MIMXRT1176_cm7)

include(driver_usdhc_MIMXRT1176_cm7)

include(middleware_wifi_common_files_MIMXRT1176_cm7)

include(middleware_freertos_backoffalgorithm_MIMXRT1176_cm7)

include(middleware_aws_iot_device_shadow_MIMXRT1176_cm7)

include(middleware_freertos_corejson_MIMXRT1176_cm7)

include(middleware_edgefast_wifi_MIMXRT1176_cm7)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


