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


project(cmsis_uart_edma_transfer)

set(MCUX_SDK_PROJECT_NAME cmsis_uart_edma_transfer.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../cmsis_usart_edma_transfer.c"
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
    ${ProjDirPath}/../../../../../../devices/MKV11Z7/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../devices/MKV11Z7/drivers
    ${ProjDirPath}/../../../../../../devices/MKV11Z7/cmsis_drivers
    ${ProjDirPath}/../../../../../../devices/MKV11Z7
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../CMSIS/Driver/Include
    ${ProjDirPath}/../../../../../../devices/MKV11Z7/utilities
)

# include modules
include(utility_debug_console_lite_MKV11Z7)

include(utility_assert_lite_MKV11Z7)

include(driver_port_MKV11Z7)

include(driver_clock_MKV11Z7)

include(driver_edma_MKV11Z7)

include(driver_dmamux_MKV11Z7)

include(driver_uart_MKV11Z7)

include(driver_uart_edma_MKV11Z7)

include(driver_cmsis_uart_MKV11Z7)

include(driver_common_MKV11Z7)

include(device_MKV11Z7_CMSIS_MKV11Z7)

include(component_uart_adapter_MKV11Z7)

include(component_lists_MKV11Z7)

include(device_MKV11Z7_startup_MKV11Z7)

include(driver_smc_MKV11Z7)

include(driver_gpio_MKV11Z7)

include(CMSIS_Include_core_cm0plus_MKV11Z7)

include(CMSIS_Include_common_MKV11Z7)

include(CMSIS_Include_dsp_MKV11Z7)

include(CMSIS_Driver_Include_USART_MKV11Z7)

include(CMSIS_Driver_Include_Common_MKV11Z7)

include(utilities_misc_utilities_MKV11Z7)

include(device_MKV11Z7_system_MKV11Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


