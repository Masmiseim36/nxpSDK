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
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/../../..
    ${ProjDirPath}/..
)

if(CMAKE_BUILD_TYPE STREQUAL debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="sss_mbedtls_frdmk64f_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="sss_mbedtls_frdmk64f_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../middleware/mmcau
    ${ProjDirPath}/../../../../../devices/MK64F12/drivers
    ${ProjDirPath}/../../../../../middleware/se_hostlib
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../components/phy
    ${ProjDirPath}/../../../../../devices/MK64F12/utilities
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../devices/MK64F12
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../rtos/freertos
)

# include modules
include(driver_mdio-enet_MK64F12)

include(driver_phy-device-ksz8081_MK64F12)

include(middleware_mmcau_cm4_cm7_MK64F12)

include(driver_rnga_MK64F12)

include(driver_i2c_MK64F12)

include(middleware_se_hostlib_cloud_enet_MK64F12)

include(middleware_lwip_enet_ethernetif_MK64F12)

include(middleware_lwip_MK64F12)

include(driver_phy-common_MK64F12)

include(driver_enet_MK64F12)

include(utility_debug_console_MK64F12)

include(driver_i2c_freertos_MK64F12)

include(driver_sim_MK64F12)

include(middleware_mbedtls_port_ksdk_MK64F12)

include(middleware_mbedtls_kinetis_MK64F12)

include(middleware_se_hostlib_commonSe050CloudDemos_MK64F12)

include(middleware_mbedtls_MK64F12)

include(driver_common_MK64F12)

include(driver_clock_MK64F12)

include(driver_pit_MK64F12)

include(driver_flash_MK64F12)

include(driver_gpio_MK64F12)

include(driver_port_MK64F12)

include(driver_uart_MK64F12)

include(driver_smc_MK64F12)

include(device_MK64F12_CMSIS_MK64F12)

include(component_uart_adapter_MK64F12)

include(component_serial_manager_MK64F12)

include(component_lists_MK64F12)

include(component_serial_manager_uart_MK64F12)

include(device_MK64F12_startup_MK64F12)

include(utility_assert_MK64F12)

include(driver_mdio-common_MK64F12)

include(CMSIS_Include_core_cm4_MK64F12)

include(CMSIS_Include_common_MK64F12)

include(CMSIS_Include_dsp_MK64F12)

include(middleware_mmcau_common_files_MK64F12)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MK64F12)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MK64F12)

include(middleware_freertos-aws_iot_common_MK64F12)

include(middleware_freertos-kernel_MK64F12)

include(middleware_freertos-kernel_extension_MK64F12)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MK64F12)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MK64F12)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MK64F12)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MK64F12)

include(middleware_freertos-aws_iot_libraries_abstractions_threading_alt_MK64F12)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_pkcs11_MK64F12)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MK64F12)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MK64F12)

include(middleware_se_hostlib_commonSe050_MK64F12)

include(middleware_se_hostlib_commonSe050_ksdk_MK64F12)

include(middleware_se_hostlib_commonSe050smCom_MK64F12)

include(middleware_se_hostlib_commonSe050infra_MK64F12)

include(middleware_se_hostlib_commonSe050_sss_ex_MK64F12)

include(middleware_se_hostlib_commonSe050_CurrentApplet_MK64F12)

include(middleware_se_hostlib_commonSe050_Scp03_MK64F12)

include(middleware_se_hostlib_commonCloudDemos_MK64F12)

include(middleware_freertos-kernel_heap_4_MK64F12)

include(middleware_freertos-aws_iot_mqtt_MK64F12)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MK64F12)

include(middleware_se_hostlib_mbedtls_sss_MK64F12)

include(middleware_se_hostlib_mbedtls_alt_demo_common_MK64F12)

include(utilities_misc_utilities_MK64F12)

include(device_MK64F12_system_MK64F12)


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


