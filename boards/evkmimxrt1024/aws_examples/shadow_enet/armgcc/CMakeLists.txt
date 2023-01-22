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


project(aws_shadow_enet)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME aws_shadow_enet.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../main_enet.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
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
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../config_files
    ${SdkRootDirPath}/boards/evkmimxrt1024
    ${ProjDirPath}/..
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/phy/device/phyksz8081
    ${SdkRootDirPath}/devices/MIMXRT1024/drivers
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/components/phy
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/components/flash/mflash/mimxrt1024
    ${SdkRootDirPath}/components/silicon_id
    ${SdkRootDirPath}/components/silicon_id/socs/rt10xx
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/kvstore
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/logging
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/mqtt_agent
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/Middleware/FreeRTOS/mqtt-agent-interface
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/evkbmimxrt1060/shadow
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/Middleware/FreeRTOS/transport_mbedtls
    ${SdkRootDirPath}/rtos/freertos/coremqtt
    ${SdkRootDirPath}/rtos/freertos/coremqtt-agent
    ${SdkRootDirPath}/rtos/freertos/corepkcs11
    ${SdkRootDirPath}/middleware/pkcs11
    ${SdkRootDirPath}/middleware/aws_iot/amazon-freertos
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/devices/MIMXRT1024
    ${SdkRootDirPath}/devices/MIMXRT1024/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1024/xip
    ${SdkRootDirPath}/boards/evkmimxrt1024/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/rtos/freertos/backoffalgorithm
    ${SdkRootDirPath}/middleware/aws_iot/device-shadow
    ${SdkRootDirPath}/rtos/freertos/corejson
)

# include modules
include(driver_phy-device-ksz8081_MIMXRT1024)

include(driver_trng_MIMXRT1024)

include(driver_dcp_MIMXRT1024)

include(driver_cache_armv7_m7_MIMXRT1024)

include(middleware_mbedtls_MIMXRT1024)

include(middleware_mbedtls_port_ksdk_MIMXRT1024)

include(middleware_mbedtls_rt_MIMXRT1024)

include(driver_flexspi_MIMXRT1024)

include(middleware_lwip_enet_ethernetif_MIMXRT1024)

include(middleware_lwip_MIMXRT1024)

include(driver_phy-common_MIMXRT1024)

include(driver_enet_MIMXRT1024)

include(component_mflash_file_MIMXRT1024)

include(component_mflash_rt1024_MIMXRT1024)

include(component_silicon_id_MIMXRT1024)

include(driver_common_MIMXRT1024)

include(component_silicon_id_rt10xx_MIMXRT1024)

include(middleware_iot_reference_kvstore_MIMXRT1024)

include(middleware_iot_reference_logging_MIMXRT1024)

include(middleware_iot_reference_mqtt_agent_MIMXRT1024)

include(middleware_iot_reference_mqtt_agent_interface_MIMXRT1024)

include(middleware_iot_reference_shadow_tasks_MIMXRT1024)

include(middleware_iot_reference_transport_mbedtls_MIMXRT1024)

include(middleware_freertos_coremqtt_MIMXRT1024)

include(middleware_freertos_coremqtt-agent_MIMXRT1024)

include(middleware_freertos_corepkcs11_mbedtls_utils_MIMXRT1024)

include(middleware_pkcs11_MIMXRT1024)

include(middleware_freertos_corepkcs11_MIMXRT1024)

include(middleware_freertos_corepkcs11_mbedtls_MIMXRT1024)

include(middleware_amazon-freertos_vendor_nxp_pkcs11_MIMXRT1024)

include(middleware_freertos-kernel_MIMXRT1024)

include(middleware_freertos-kernel_heap_4_MIMXRT1024)

include(driver_clock_MIMXRT1024)

include(device_MIMXRT1024_CMSIS_MIMXRT1024)

include(utility_debug_console_MIMXRT1024)

include(component_lpuart_adapter_MIMXRT1024)

include(component_serial_manager_MIMXRT1024)

include(component_lists_MIMXRT1024)

include(component_serial_manager_uart_MIMXRT1024)

include(driver_lpuart_MIMXRT1024)

include(device_MIMXRT1024_startup_MIMXRT1024)

include(driver_iomuxc_MIMXRT1024)

include(utility_assert_MIMXRT1024)

include(driver_igpio_MIMXRT1024)

include(driver_xip_device_MIMXRT1024)

include(driver_xip_board_evkmimxrt1024_MIMXRT1024)

include(CMSIS_Include_core_cm_MIMXRT1024)

include(component_mflash_common_MIMXRT1024)

include(middleware_freertos-kernel_extension_MIMXRT1024)

include(middleware_freertos_backoffalgorithm_MIMXRT1024)

include(middleware_aws_iot_device_shadow_MIMXRT1024)

include(middleware_freertos_corejson_MIMXRT1024)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT1024)

include(utilities_misc_utilities_MIMXRT1024)

include(device_MIMXRT1024_system_MIMXRT1024)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


