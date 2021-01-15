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


project(azure_mqtt_rc)

set(MCUX_SDK_PROJECT_NAME azure_mqtt_rc.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../azure_mqtt_rc.c"
"${ProjDirPath}/../azure_mqtt_rc.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../azure_mbedtls_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. sdram_debug
    ${ProjDirPath}/..
)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="azure_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="azure_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="azure_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="azure_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../components/phy
    ${ProjDirPath}/../../../../../middleware/azure_iot
    ${ProjDirPath}/../..
    ${ProjDirPath}/../../../../../devices/MIMXRT1052
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(driver_mdio-enet_MIMXRT1052)

include(driver_phy-device-ksz8081_MIMXRT1052)

include(driver_trng_MIMXRT1052)

include(driver_dcp_MIMXRT1052)

include(driver_cache_armv7_m7_MIMXRT1052)

include(middleware_mbedtls_port_ksdk_MIMXRT1052)

include(middleware_mbedtls_rt_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(middleware_freertos-kernel_heap_3_MIMXRT1052)

include(middleware_mbedtls_MIMXRT1052)

include(utility_debug_console_MIMXRT1052)

include(middleware_lwip_apps_sntp_MIMXRT1052)

include(middleware_lwip_MIMXRT1052)

include(middleware_lwip_enet_ethernetif_MIMXRT1052)

include(driver_phy-common_MIMXRT1052)

include(driver_enet_MIMXRT1052)

include(middleware_azure_iot_iothub_client_mqtt_MIMXRT1052)

include(middleware_azure_iot_java_app_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(component_serial_manager_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(component_serial_manager_uart_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(utility_assert_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_MIMXRT1052)

include(driver_mdio-common_MIMXRT1052)

include(CMSIS_Include_core_cm7_MIMXRT1052)

include(CMSIS_Include_common_MIMXRT1052)

include(CMSIS_Include_dsp_MIMXRT1052)

include(middleware_freertos-kernel_MIMXRT1052)

include(middleware_freertos-kernel_extension_MIMXRT1052)

include(middleware_azure_iot_MIMXRT1052)

include(middleware_azure_iot_mqtt_MIMXRT1052)

include(middleware_azure_iot_adapters_MIMXRT1052)

include(middleware_azure_iot_adapters_freertos_MIMXRT1052)

include(middleware_azure_iot_iothub_client_MIMXRT1052)

include(middleware_azure_iot_certs_MIMXRT1052)

include(middleware_azure_iot_parson_MIMXRT1052)

include(middleware_azure_iot_umock_c_MIMXRT1052)

include(middleware_azure_iot_azure_macro_utils_MIMXRT1052)

include(middleware_azure_iot_mbedtls_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE g)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE stdc++)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


