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


project(cmsis_enet_txrx_transfer)

set(MCUX_SDK_PROJECT_NAME cmsis_enet_txrx_transfer.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../cmsis_enet_txrx_transfer.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../RTE_Device.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../../devices/MK66F18/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../devices/MK66F18/drivers
    ${ProjDirPath}/../../../../../../devices/MK66F18/cmsis_drivers
    ${ProjDirPath}/../../../../../../devices/MK66F18
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../components/phy
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../CMSIS/Driver/Include
    ${ProjDirPath}/../../../../../../devices/MK66F18/utilities
)

# include modules
include(driver_mdio-enet_MK66F18)

include(driver_phy-device-ksz8081_MK66F18)

include(utility_debug_console_lite_MK66F18)

include(utility_assert_lite_MK66F18)

include(driver_port_MK66F18)

include(driver_sysmpu_MK66F18)

include(driver_sim_MK66F18)

include(driver_gpio_MK66F18)

include(driver_clock_MK66F18)

include(driver_enet_MK66F18)

include(driver_cmsis_enet_MK66F18)

include(driver_common_MK66F18)

include(driver_flash_MK66F18)

include(device_MK66F18_CMSIS_MK66F18)

include(component_uart_adapter_MK66F18)

include(component_lists_MK66F18)

include(driver_uart_MK66F18)

include(device_MK66F18_startup_MK66F18)

include(driver_lpuart_MK66F18)

include(driver_smc_MK66F18)

include(driver_mdio-common_MK66F18)

include(CMSIS_Include_core_cm4_MK66F18)

include(CMSIS_Include_common_MK66F18)

include(CMSIS_Include_dsp_MK66F18)

include(driver_phy-common_MK66F18)

include(CMSIS_Driver_Include_Ethernet_MAC_MK66F18)

include(CMSIS_Driver_Include_Ethernet_MK66F18)

include(CMSIS_Driver_Include_Common_MK66F18)

include(CMSIS_Driver_Include_Ethernet_PHY_MK66F18)

include(driver_edma_MK66F18)

include(driver_dmamux_MK66F18)

include(utilities_misc_utilities_MK66F18)

include(device_MK66F18_system_MK66F18)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


