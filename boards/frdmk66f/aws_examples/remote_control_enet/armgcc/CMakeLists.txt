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


project(aws_remote_control_enet)

set(MCUX_SDK_PROJECT_NAME aws_remote_control_enet.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../remote_control.c"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../lwipopts.h"
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
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../config_files debug
    ${ProjDirPath}/../../.. debug
    ${ProjDirPath}/../config_files
    ${ProjDirPath}/..
    ${ProjDirPath}/../../..
)

if(CMAKE_BUILD_TYPE STREQUAL debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../devices/MK66F18/drivers
    ${ProjDirPath}/../../../../../middleware/mmcau
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../components/fxos8700cq
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../components/phy
    ${ProjDirPath}/../../../../../devices/MK66F18/utilities
    ${ProjDirPath}/../../../../../components/flash/mflash
    ${ProjDirPath}/../../../../../components/flash/mflash/frdmk66f
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../devices/MK66F18
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(driver_mdio-enet_MK66F18)

include(driver_phy-device-ksz8081_MK66F18)

include(driver_rnga_MK66F18)

include(middleware_mmcau_cm4_cm7_MK66F18)

include(middleware_mbedtls_port_ksdk_MK66F18)

include(middleware_mbedtls_kinetis_MK66F18)

include(driver_fxos8700cq_MK66F18)

include(driver_i2c_MK66F18)

include(middleware_lwip_enet_ethernetif_MK66F18)

include(middleware_lwip_MK66F18)

include(driver_phy-common_MK66F18)

include(driver_enet_MK66F18)

include(utility_debug_console_MK66F18)

include(component_mflash_file_MK66F18)

include(component_mflash_frdmk66f_MK66F18)

include(driver_port_MK66F18)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MK66F18)

include(middleware_freertos-aws_iot_demos_MK66F18)

include(middleware_freertos-aws_iot_demos_dev_mode_key_provisioning_MK66F18)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MK66F18)

include(middleware_freertos-kernel_MK66F18)

include(middleware_freertos-kernel_heap_4_MK66F18)

include(middleware_freertos-aws_iot_c_sdk_standard_shadow_MK66F18)

include(middleware_freertos-aws_iot_c_sdk_standard_serializer_MK66F18)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MK66F18)

include(middleware_freertos-aws_iot_vendor_nxp_pkcs11_MK66F18)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MK66F18)

include(middleware_mbedtls_MK66F18)

include(driver_clock_MK66F18)

include(driver_flash_MK66F18)

include(driver_common_MK66F18)

include(device_MK66F18_CMSIS_MK66F18)

include(component_uart_adapter_MK66F18)

include(component_serial_manager_MK66F18)

include(component_lists_MK66F18)

include(component_serial_manager_uart_MK66F18)

include(driver_uart_MK66F18)

include(device_MK66F18_startup_MK66F18)

include(driver_lpuart_MK66F18)

include(driver_smc_MK66F18)

include(driver_gpio_MK66F18)

include(utility_assert_MK66F18)

include(driver_mdio-common_MK66F18)

include(CMSIS_Include_core_cm4_MK66F18)

include(CMSIS_Include_common_MK66F18)

include(CMSIS_Include_dsp_MK66F18)

include(middleware_mmcau_common_files_MK66F18)

include(component_mflash_common_MK66F18)

include(driver_cache_lmem_MK66F18)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MK66F18)

include(middleware_freertos-aws_iot_common_MK66F18)

include(middleware_freertos-kernel_extension_MK66F18)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MK66F18)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MK66F18)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MK66F18)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MK66F18)

include(middleware_freertos-aws_iot_libraries_abstractions_threading_alt_MK66F18)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11_MK66F18)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MK66F18)

include(middleware_freertos-aws_iot_network_manager_MK66F18)

include(middleware_freertos-aws_iot_mqtt_MK66F18)

include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MK66F18)

include(utilities_misc_utilities_MK66F18)

include(device_MK66F18_system_MK66F18)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


