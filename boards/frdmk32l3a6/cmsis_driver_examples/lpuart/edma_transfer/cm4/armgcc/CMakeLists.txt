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


project(cmsis_lpuart_edma_transfer_cm4)

set(MCUX_SDK_PROJECT_NAME cmsis_lpuart_edma_transfer_cm4.elf)

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
    ${ProjDirPath}/../../../../../../../devices/K32L3A60/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../../devices/K32L3A60/drivers
    ${ProjDirPath}/../../../../../../../devices/K32L3A60/cmsis_drivers
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/K32L3A60
    ${ProjDirPath}/../../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../../devices
    ${ProjDirPath}/../../../../../../../CMSIS/Driver/Include
    ${ProjDirPath}/../../../../../../../devices/K32L3A60/utilities
)

# include modules
include(utility_debug_console_lite_K32L3A60_cm4)

include(utility_assert_lite_K32L3A60_cm4)

include(driver_port_K32L3A60_cm4)

include(driver_clock_K32L3A60_cm4)

include(driver_edma_K32L3A60_cm4)

include(driver_dmamux_K32L3A60_cm4)

include(driver_lpuart_K32L3A60_cm4)

include(driver_lpuart_edma_K32L3A60_cm4)

include(driver_cmsis_lpuart_K32L3A60_cm4)

include(driver_common_K32L3A60_cm4)

include(driver_msmc_K32L3A60_cm4)

include(driver_gpio_K32L3A60_cm4)

include(component_lpuart_adapter_K32L3A60_cm4)

include(component_lists_K32L3A60_cm4)

include(device_K32L3A60_CMSIS_K32L3A60_cm4)

include(CMSIS_Include_core_cm4_K32L3A60_cm4)

include(device_K32L3A60_startup_K32L3A60_cm4)

include(CMSIS_Driver_Include_USART_K32L3A60_cm4)

include(CMSIS_Driver_Include_Common_K32L3A60_cm4)

include(utilities_misc_utilities_K32L3A60_cm4)

include(CMSIS_Include_common_K32L3A60_cm4)

include(CMSIS_Include_dsp_K32L3A60_cm4)

include(device_K32L3A60_system_K32L3A60_cm4)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


