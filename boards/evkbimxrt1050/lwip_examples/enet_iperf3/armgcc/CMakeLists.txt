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


project(lwip_enet_iperf3)

set(MCUX_SDK_PROJECT_NAME lwip_enet_iperf3.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../iperf_lwip.c"
"${ProjDirPath}/../iperf_api.h"
"${ProjDirPath}/../aws_wifi_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. flexspi_nor_debug
    ${ProjDirPath}/../../.. flexspi_nor_debug
    ${ProjDirPath}/..
    ${ProjDirPath}/../../.. flexspi_nor_release
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers
    ${ProjDirPath}/../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../components/phy
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../devices/MIMXRT1052
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(driver_cache_armv7_m7_MIMXRT1052)

include(driver_mdio-enet_MIMXRT1052)

include(driver_phy-device-ksz8081_MIMXRT1052)

include(middleware_lwip_enet_ethernetif_MIMXRT1052)

include(middleware_lwip_MIMXRT1052)

include(driver_phy-common_MIMXRT1052)

include(driver_enet_MIMXRT1052)

include(utility_debug_console_MIMXRT1052)

include(middleware_freertos-kernel_heap_4_MIMXRT1052)

include(middleware_freertos-kernel_MIMXRT1052)

include(driver_clock_MIMXRT1052)

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

include(CMSIS_Include_core_cm7_MIMXRT1052)

include(CMSIS_Include_common_MIMXRT1052)

include(CMSIS_Include_dsp_MIMXRT1052)

include(driver_mdio-common_MIMXRT1052)

include(middleware_freertos-kernel_extension_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


