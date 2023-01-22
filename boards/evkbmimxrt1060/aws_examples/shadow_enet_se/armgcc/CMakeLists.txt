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


project(aws_shadow_enet_se)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME aws_shadow_enet_se.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
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
"${ProjDirPath}/../aws_mbedtls_config.h"
"${ProjDirPath}/../core_mqtt_config.h"
"${ProjDirPath}/../core_pkcs11_config.h"
"${ProjDirPath}/../demo_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../fsl_sss_ftr.h"
"${ProjDirPath}/../kvstore_config.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../ota_config.h"
"${ProjDirPath}/../shadow_config.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../app_main.c"
"${ProjDirPath}/../ota/ota_pal.c"
"${ProjDirPath}/../ota/ota_pal.h"
"${ProjDirPath}/../ota/ota_signature_validation.c"
"${ProjDirPath}/../ota/ota_update.c"
"${ProjDirPath}/../ota/ota_update.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkbmimxrt1060
    ${ProjDirPath}/../bootutil_port
    ${ProjDirPath}/../bootutil_port/mcuboot_config
    ${ProjDirPath}/../bootutil_port/sysflash
    ${ProjDirPath}/../bootutil_port/board_specific
    ${ProjDirPath}/../ota
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h" MBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h" MBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/phy/device/phyksz8081
    ${SdkRootDirPath}/devices/MIMXRT1062/drivers
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/components/phy
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/components/flash/mflash/mimxrt1062
    ${SdkRootDirPath}/components/silicon_id
    ${SdkRootDirPath}/components/silicon_id/socs/rt10xx
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/cli
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/kvstore
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/logging
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/mqtt_agent
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/evkbmimxrt1060/shadow
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/Middleware/FreeRTOS/transport_mbedtls
    ${SdkRootDirPath}/middleware/aws_iot/ota
    ${SdkRootDirPath}/middleware/se_hostlib
    ${SdkRootDirPath}/middleware/pkcs11
    ${SdkRootDirPath}/rtos/freertos/corepkcs11
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/devices/MIMXRT1062
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/Middleware/FreeRTOS/mqtt-agent-interface
    ${SdkRootDirPath}/rtos/freertos/coremqtt-agent
    ${SdkRootDirPath}/rtos/freertos/coremqtt
    ${SdkRootDirPath}/rtos/freertos/backoffalgorithm
    ${SdkRootDirPath}/middleware/aws_iot/device-shadow
    ${SdkRootDirPath}/rtos/freertos/corejson
    ${SdkRootDirPath}/middleware/tinycbor
)

# include modules
include(driver_phy-device-ksz8081_MIMXRT1062)

include(driver_trng_MIMXRT1062)

include(driver_dcp_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

include(middleware_mbedtls_MIMXRT1062)

include(middleware_mbedtls_port_ksdk_MIMXRT1062)

include(middleware_mbedtls_rt_MIMXRT1062)

include(driver_flexspi_MIMXRT1062)

include(middleware_lwip_enet_ethernetif_MIMXRT1062)

include(middleware_lwip_MIMXRT1062)

include(driver_phy-common_MIMXRT1062)

include(driver_enet_MIMXRT1062)

include(component_mflash_file_MIMXRT1062)

include(component_mflash_rt1060_MIMXRT1062)

include(driver_lpi2c_freertos_MIMXRT1062)

include(component_silicon_id_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(component_silicon_id_rt10xx_MIMXRT1062)

include(middleware_iot_reference_cli_MIMXRT1062)

include(middleware_iot_reference_kvstore_MIMXRT1062)

include(middleware_iot_reference_logging_MIMXRT1062)

include(middleware_iot_reference_mqtt_agent_MIMXRT1062)

include(middleware_iot_reference_shadow_tasks_MIMXRT1062)

include(middleware_iot_reference_transport_mbedtls_MIMXRT1062)

include(middleware_aws_iot_ota_MIMXRT1062)

include(middleware_aws_iot_ota_freertos_MIMXRT1062)

include(middleware_se_hostlib_commonSe050CloudDemos_MIMXRT1062)

include(middleware_pkcs11_MIMXRT1062)

include(middleware_freertos_corepkcs11_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(middleware_freertos-kernel_heap_4_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(utility_debug_console_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_serial_manager_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_serial_manager_uart_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(utility_assert_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(component_mflash_common_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(middleware_freertos-kernel_extension_MIMXRT1062)

include(middleware_iot_reference_mqtt_agent_interface_MIMXRT1062)

include(middleware_freertos_coremqtt-agent_MIMXRT1062)

include(middleware_freertos_coremqtt_MIMXRT1062)

include(middleware_freertos_backoffalgorithm_MIMXRT1062)

include(middleware_aws_iot_device_shadow_MIMXRT1062)

include(middleware_freertos_corejson_MIMXRT1062)

include(middleware_tinycbor_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_ksdk_MIMXRT1062)

include(middleware_se_hostlib_commonSe050smCom_MIMXRT1062)

include(middleware_se_hostlib_commonSe050infra_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_sss_ex_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_CurrentApplet_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_Scp03_MIMXRT1062)

include(middleware_se_hostlib_mwlog_MIMXRT1062)

include(middleware_se_hostlib_commonCloudDemos_MIMXRT1062)

include(middleware_se_hostlib_mbedtls_sss_MIMXRT1062)

include(middleware_se_hostlib_mbedtls_alt_demo_common_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/shadow_enet_se.bin)

