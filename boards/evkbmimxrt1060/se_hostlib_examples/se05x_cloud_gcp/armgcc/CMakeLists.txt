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


project(se05x_cloud_gcp)

set(MCUX_SDK_PROJECT_NAME se05x_cloud_gcp.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../fsl_sss_ftr.h"
"${ProjDirPath}/../gcpJWTDemo.c"
"${ProjDirPath}/../gcpJWTDemo.h"
"${ProjDirPath}/../gcpJWTDemo_Main_lwip.c"
"${ProjDirPath}/../gcpJwtTask_lwip.c"
"${ProjDirPath}/../gcpNTPGetEpoch_lwip.c"
"${ProjDirPath}/../gcpPubSub.c"
"${ProjDirPath}/../gcp_client_credential_keys.h"
"${ProjDirPath}/../gcp_default_root_certificates.h"
"${ProjDirPath}/../gcp_iot_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../..
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="sss_mbedtls_evkbmimxrt1060_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="sss_mbedtls_evkbmimxrt1060_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../components/phy
    ${ProjDirPath}/../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../middleware/se_hostlib
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../rtos/freertos
)

# include modules
include(driver_trng_MIMXRT1062)

include(driver_lpi2c_freertos_MIMXRT1062)

include(driver_dcp_MIMXRT1062)

include(driver_pit_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

include(driver_phy-common_MIMXRT1062)

include(driver_phy-device-ksz8081_MIMXRT1062)

include(driver_mdio-enet_MIMXRT1062)

include(driver_flexspi_MIMXRT1062)

include(middleware_se_hostlib_cloud_enet_evkbmimxrt1060_MIMXRT1062)

include(middleware_lwip_enet_ethernetif_MIMXRT1062)

include(middleware_lwip_MIMXRT1062)

include(driver_enet_MIMXRT1062)

include(utility_debug_console_MIMXRT1062)

include(middleware_mbedtls_port_ksdk_MIMXRT1062)

include(middleware_mbedtls_MIMXRT1062)

include(middleware_mbedtls_rt_MIMXRT1062)

include(middleware_se_hostlib_commonSe050CloudDemos_evkbmimxrt1060_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_serial_manager_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_serial_manager_uart_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(utility_assert_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkbmimxrt1060_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(middleware_freertos-kernel_extension_MIMXRT1062)

include(driver_mdio-common_MIMXRT1062)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_config_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_ksdk_MIMXRT1062)

include(middleware_se_hostlib_commonSe050smCom_MIMXRT1062)

include(middleware_se_hostlib_commonSe050infra_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_sss_ex_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_CurrentApplet_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_Scp03_MIMXRT1062)

include(middleware_se_hostlib_mwlog_MIMXRT1062)

include(middleware_se_hostlib_commonCloudDemos_evkbmimxrt1060_MIMXRT1062)

include(middleware_freertos-kernel_heap_4_MIMXRT1062)

include(middleware_freertos-aws_iot_mqtt_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_logging_MIMXRT1062)

include(middleware_se_hostlib_mbedtls_sss_MIMXRT1062)

include(middleware_se_hostlib_mbedtls_alt_demo_common_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


