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


project(se05x_iot_agent_demo)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME se05x_iot_agent_demo.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../fsl_sss_ftr.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../aws_secure_sockets_config.h"
"${ProjDirPath}/../aws_mqtt_config.h"
"${ProjDirPath}/../aws_bufferpool_config.h"
"${ProjDirPath}/../iot_mqtt_agent_config.h"
"${SdkRootDirPath}/boards/evkmimxrt1060/se_hostlib_examples/se05x_cloud_gcp/FreeRTOSIPConfig.h"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../iot_agent_demo.c"
"${ProjDirPath}/../core_mqtt_config.h"
"${ProjDirPath}/../core_pkcs11_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../FreeRTOSConfig.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt1060
    ${SdkRootDirPath}/middleware/se_hostlib/demos/ksdk/se05x/iot_agent
    ${SdkRootDirPath}/middleware/se_hostlib/sss/plugin/mbedtls
    ${SdkRootDirPath}/middleware/se_hostlib/hostlib/hostLib/tstUtil
    ${SdkRootDirPath}/middleware/se_hostlib/demos/ksdk/common
    ${SdkRootDirPath}/middleware/se_hostlib/nxp_iot_agent/ex/inc
    ${SdkRootDirPath}/middleware/se_hostlib/nxp_iot_agent/platform
    ${SdkRootDirPath}/middleware/se_hostlib/nxp_iot_agent/platform/mbedtls
    ${SdkRootDirPath}/middleware/se_hostlib/semslite/lib/inc
    ${SdkRootDirPath}/middleware/se_hostlib/nxp_iot_agent/ex/src/utils
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h" MBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h" MBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1062/drivers
    ${SdkRootDirPath}/components/phy
    ${SdkRootDirPath}/components/phy/device/phyksz8081
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/middleware/se_hostlib
    ${SdkRootDirPath}/middleware/aws_iot/amazon-freertos
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/Middleware/FreeRTOS/transport_mbedtls
    ${SdkRootDirPath}/middleware/aws_iot/iot-reference/examples/common/logging
    ${SdkRootDirPath}/devices/MIMXRT1062
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1062/xip
    ${SdkRootDirPath}/boards/evkmimxrt1060/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/rtos/freertos/coremqtt
    ${SdkRootDirPath}/middleware/pkcs11
    ${SdkRootDirPath}/rtos/freertos/backoffalgorithm
    ${SdkRootDirPath}/rtos/freertos/corepkcs11
)

# include modules
include(driver_trng_MIMXRT1062)

include(driver_lpi2c_freertos_MIMXRT1062)

include(driver_dcp_MIMXRT1062)

include(driver_pit_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

include(driver_phy-common_MIMXRT1062)

include(driver_phy-device-ksz8081_MIMXRT1062)

include(driver_flexspi_MIMXRT1062)

include(middleware_lwip_enet_ethernetif_MIMXRT1062)

include(middleware_lwip_MIMXRT1062)

include(driver_enet_MIMXRT1062)

include(middleware_mbedtls_port_ksdk_MIMXRT1062)

include(middleware_mbedtls_MIMXRT1062)

include(middleware_mbedtls_rt_MIMXRT1062)

include(middleware_se_hostlib_nxp_iot_agent_lib_MIMXRT1062)

include(middleware_amazon-freertos_libraries_3rdparty_jsmn_MIMXRT1062)

include(middleware_iot_reference_transport_mbedtls_MIMXRT1062)

include(middleware_iot_reference_logging_MIMXRT1062)

include(driver_common_MIMXRT1062)

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

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkmimxrt1060_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(middleware_freertos-kernel_extension_MIMXRT1062)

include(middleware_se_hostlib_commonSe050CloudDemos_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_ksdk_MIMXRT1062)

include(middleware_se_hostlib_commonSe050smCom_MIMXRT1062)

include(middleware_se_hostlib_commonSe050infra_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_sss_ex_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_CurrentApplet_MIMXRT1062)

include(middleware_se_hostlib_commonSe050_Scp03_MIMXRT1062)

include(middleware_se_hostlib_mwlog_MIMXRT1062)

include(middleware_se_hostlib_commonCloudDemos_MIMXRT1062)

include(middleware_freertos-kernel_heap_4_MIMXRT1062)

include(middleware_freertos_coremqtt_MIMXRT1062)

include(middleware_pkcs11_MIMXRT1062)

include(middleware_freertos_backoffalgorithm_MIMXRT1062)

include(middleware_se_hostlib_mbedtls_sss_MIMXRT1062)

include(middleware_se_hostlib_mbedtls_alt_demo_common_MIMXRT1062)

include(middleware_freertos_corepkcs11_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


