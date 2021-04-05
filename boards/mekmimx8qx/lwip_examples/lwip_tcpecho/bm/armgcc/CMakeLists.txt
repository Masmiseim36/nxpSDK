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


project(lwip_tcpecho_bm)

set(MCUX_SDK_PROJECT_NAME lwip_tcpecho_bm.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lwip_tcpecho_bm.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../empty_rsc_table.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMX8QX6/drivers
    ${ProjDirPath}/../../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../../components/phy/device/phyar8031
    ${ProjDirPath}/../../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../../components/phy
    ${ProjDirPath}/../../../../../../devices/MIMX8QX6/utilities
    ${ProjDirPath}/../../../../../../devices/MIMX8QX6
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../devices/MIMX8QX6/scfw_api
    ${ProjDirPath}/../../../../../../CMSIS/Include
)

# include modules
include(driver_irqsteer_MIMX8QX6)

include(driver_memory_MIMX8QX6)

include(driver_mdio-enet_MIMX8QX6)

include(driver_phy-device-ar8031_MIMX8QX6)

include(driver_cache_lmem_MIMX8QX6)

include(middleware_lwip_contrib_tcpecho_raw_MIMX8QX6)

include(middleware_lwip_enet_ethernetif_MIMX8QX6)

include(middleware_lwip_MIMX8QX6)

include(driver_phy-common_MIMX8QX6)

include(driver_enet_MIMX8QX6)

include(utility_debug_console_MIMX8QX6)

include(driver_clock_MIMX8QX6)

include(driver_common_MIMX8QX6)

include(device_MIMX8QX6_CMSIS_MIMX8QX6)

include(component_lpuart_adapter_MIMX8QX6)

include(component_serial_manager_MIMX8QX6)

include(component_lists_MIMX8QX6)

include(component_serial_manager_uart_MIMX8QX6)

include(driver_lpuart_MIMX8QX6)

include(device_MIMX8QX6_startup_MIMX8QX6)

include(driver_scfw_api_MIMX8QX6)

include(driver_igpio_MIMX8QX6)

include(utility_assert_MIMX8QX6)

include(CMSIS_Include_core_cm4_MIMX8QX6)

include(CMSIS_Include_common_MIMX8QX6)

include(CMSIS_Include_dsp_MIMX8QX6)

include(driver_mdio-common_MIMX8QX6)

include(utilities_misc_utilities_MIMX8QX6)

include(device_MIMX8QX6_system_MIMX8QX6)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/lwip_tcpecho_bm.bin)

