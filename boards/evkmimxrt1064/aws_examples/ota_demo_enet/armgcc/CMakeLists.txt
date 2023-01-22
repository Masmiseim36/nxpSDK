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


project(ota_demo_enet)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME ota_demo_enet.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../main_enet.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
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
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../config_files
    ${SdkRootDirPath}/boards/evkmimxrt1064
    ${ProjDirPath}/..
    ${ProjDirPath}/../bootutil_port
    ${ProjDirPath}/../bootutil_port/mcuboot_config
    ${ProjDirPath}/../bootutil_port/sysflash
    ${ProjDirPath}/../bootutil_port/board_specific
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/phy/device/phyksz8081
    ${SdkRootDirPath}/devices/MIMXRT1064/drivers
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/components/phy
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/components/flash/mflash/mimxrt1064
    ${SdkRootDirPath}/components/silicon_id
    ${SdkRootDirPath}/components/silicon_id/socs/rt10xx
    ${SdkRootDirPath}/middleware/aws_iot/amazon-freertos
    ${SdkRootDirPath}/middleware/aws_iot/ota
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/rtos/freertos/coremqtt
    ${SdkRootDirPath}/middleware/pkcs11
    ${SdkRootDirPath}/rtos/freertos/corepkcs11
    ${SdkRootDirPath}/devices/MIMXRT1064
    ${SdkRootDirPath}/devices/MIMXRT1064/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1064/xip
    ${SdkRootDirPath}/boards/evkmimxrt1064/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/rtos/freertos/corejson
    ${SdkRootDirPath}/middleware/tinycbor
    ${SdkRootDirPath}/rtos/freertos/backoffalgorithm
    ${SdkRootDirPath}/rtos/freertos/coremqtt-agent
)

# include modules
include(driver_phy-device-ksz8081_MIMXRT1064)

include(driver_cache_armv7_m7_MIMXRT1064)

include(middleware_mbedtls_MIMXRT1064)

include(middleware_mbedtls_port_ksdk_MIMXRT1064)

include(middleware_mbedtls_rt_MIMXRT1064)

include(driver_flexspi_MIMXRT1064)

include(middleware_lwip_enet_ethernetif_MIMXRT1064)

include(middleware_lwip_MIMXRT1064)

include(driver_phy-common_MIMXRT1064)

include(driver_enet_MIMXRT1064)

include(component_mflash_file_MIMXRT1064)

include(component_mflash_rt1064_MIMXRT1064)

include(component_silicon_id_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(component_silicon_id_rt10xx_MIMXRT1064)

include(middleware_amazon-freertos_demos_MIMXRT1064)

include(middleware_amazon-freertos_demos_dev_mode_key_provisioning_MIMXRT1064)

include(middleware_amazon-freertos_demos_ota_mqtt_MIMXRT1064)

include(middleware_aws_iot_ota_MIMXRT1064)

include(middleware_amazon-freertos_vendor_nxp_secure_sockets_lwip_MIMXRT1064)

include(middleware_amazon-freertos_vendor_nxp_ota_pal_MIMXRT1064)

include(middleware_amazon-freertos_libraries_abstractions_transport_MIMXRT1064)

include(middleware_freertos-kernel_MIMXRT1064)

include(middleware_amazon-freertos_libraries_abstractions_secure_sockets_MIMXRT1064)

include(middleware_amazon-freertos_libraries_logging_MIMXRT1064)

include(middleware_freertos_coremqtt_MIMXRT1064)

include(middleware_pkcs11_MIMXRT1064)

include(middleware_freertos_corepkcs11_MIMXRT1064)

include(middleware_freertos_corepkcs11_mbedtls_MIMXRT1064)

include(middleware_amazon-freertos_vendor_nxp_pkcs11_MIMXRT1064)

include(middleware_freertos-kernel_heap_4_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(utility_debug_console_MIMXRT1064)

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

include(driver_dcp_MIMXRT1064)

include(driver_trng_MIMXRT1064)

include(component_mflash_common_MIMXRT1064)

include(middleware_amazon-freertos_libraries_abstractions_platform_MIMXRT1064)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT1064)

include(middleware_freertos-kernel_extension_MIMXRT1064)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_include_MIMXRT1064)

include(middleware_amazon-freertos_network_manager_MIMXRT1064)

include(middleware_amazon-freertos_demos_ota_demo_helpers_MIMXRT1064)

include(middleware_freertos_corejson_MIMXRT1064)

include(middleware_tinycbor_MIMXRT1064)

include(middleware_freertos_backoffalgorithm_MIMXRT1064)

include(middleware_amazon-freertos_libraries_abstractions_mqtt_agent_MIMXRT1064)

include(middleware_freertos_coremqtt-agent_MIMXRT1064)

include(middleware_aws_iot_ota_freertos_MIMXRT1064)

include(middleware_amazon-freertos_pkcs11_helpers_MIMXRT1064)

include(middleware_amazon-freertos_demos_common_mqtt_subscription_manager_MIMXRT1064)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_tls_MIMXRT1064)

include(middleware_amazon-freertos_libraries_3rdparty_mbedtls_utils_MIMXRT1064)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT1064)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_utils_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/ota_demo.bin)

