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


project(lwip_httpscli_ota_enet)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME lwip_httpscli_ota_enet.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lwip_httpscli_ota.c"
"${ProjDirPath}/../mbedtls_config_client.h"
"${ProjDirPath}/../flash_helper.h"
"${ProjDirPath}/../flash_helper.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../init_enet.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../httpsclient.c"
"${ProjDirPath}/../httpsclient.h"
"${ProjDirPath}/../ota_http.c"
"${ProjDirPath}/../ota_http.h"
"${ProjDirPath}/../ota_config.h"
"${ProjDirPath}/../core_http_config.c"
"${ProjDirPath}/../core_http_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../network_cfg.h"
"${ProjDirPath}/../network_board_cfg.h"
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
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt1040
    ${ProjDirPath}/../bootutil_port
    ${ProjDirPath}/../bootutil_port/mcuboot_config
    ${ProjDirPath}/../bootutil_port/sysflash
    ${ProjDirPath}/../bootutil_port/board_specific
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="mbedtls_config_client.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="mbedtls_config_client.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/phy/device/phyksz8081
    ${SdkRootDirPath}/devices/MIMXRT1042/drivers
    ${SdkRootDirPath}/middleware/mbedtls
    ${SdkRootDirPath}/components/flash/mflash/mimxrt1042
    ${SdkRootDirPath}/components/silicon_id
    ${SdkRootDirPath}/components/silicon_id/socs/rt10xx
    ${SdkRootDirPath}/middleware/lwip
    ${SdkRootDirPath}/components/phy
    ${SdkRootDirPath}/rtos/freertos/corehttp
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/devices/MIMXRT1042/utilities
    ${SdkRootDirPath}/devices/MIMXRT1042
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1042/xip
    ${SdkRootDirPath}/boards/evkmimxrt1040/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/flash/mflash
    ${SdkRootDirPath}/middleware/llhttp
)

# include modules
include(driver_phy-device-ksz8081_MIMXRT1042)

include(driver_trng_MIMXRT1042)

include(driver_dcp_MIMXRT1042)

include(driver_cache_armv7_m7_MIMXRT1042)

include(middleware_mbedtls_MIMXRT1042)

include(middleware_mbedtls_port_ksdk_MIMXRT1042)

include(middleware_mbedtls_rt_MIMXRT1042)

include(component_mflash_rt1040_MIMXRT1042)

include(component_silicon_id_MIMXRT1042)

include(driver_common_MIMXRT1042)

include(component_silicon_id_rt10xx_MIMXRT1042)

include(middleware_lwip_enet_ethernetif_MIMXRT1042)

include(middleware_lwip_MIMXRT1042)

include(driver_phy-common_MIMXRT1042)

include(driver_enet_MIMXRT1042)

include(middleware_freertos_corehttp_MIMXRT1042)

include(driver_clock_MIMXRT1042)

include(middleware_freertos-kernel_heap_4_MIMXRT1042)

include(utility_shell_MIMXRT1042)

include(device_MIMXRT1042_CMSIS_MIMXRT1042)

include(utility_debug_console_MIMXRT1042)

include(component_lpuart_adapter_MIMXRT1042)

include(component_serial_manager_MIMXRT1042)

include(component_lists_MIMXRT1042)

include(component_serial_manager_uart_MIMXRT1042)

include(driver_lpuart_MIMXRT1042)

include(device_MIMXRT1042_startup_MIMXRT1042)

include(driver_iomuxc_MIMXRT1042)

include(utility_assert_MIMXRT1042)

include(driver_igpio_MIMXRT1042)

include(driver_xip_device_MIMXRT1042)

include(driver_xip_board_evkmimxrt1040_MIMXRT1042)

include(CMSIS_Include_core_cm_MIMXRT1042)

include(component_mflash_common_MIMXRT1042)

include(driver_flexspi_MIMXRT1042)

include(middleware_llhttp_MIMXRT1042)

include(middleware_freertos-kernel_MIMXRT1042)

include(middleware_freertos-kernel_extension_MIMXRT1042)

include(utilities_misc_utilities_MIMXRT1042)

include(device_MIMXRT1042_system_MIMXRT1042)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/lwip_httpscli_ota.bin)

