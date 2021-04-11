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


project(lpspi_interrupt_b2b_transfer_master_cm0plus)

set(MCUX_SDK_PROJECT_NAME lpspi_interrupt_b2b_transfer_master_cm0plus.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lpspi_interrupt_b2b_transfer_master.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers
    ${ProjDirPath}/../../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60
    ${ProjDirPath}/../../../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../../../devices
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities
)

# include modules
include(utility_debug_console_lite_K32L3A60_cm0plus)

include(utility_assert_lite_K32L3A60_cm0plus)

include(driver_port_K32L3A60_cm0plus)

include(driver_lpspi_K32L3A60_cm0plus)

include(driver_clock_K32L3A60_cm0plus)

include(driver_common_K32L3A60_cm0plus)

include(driver_msmc_K32L3A60_cm0plus)

include(driver_lpuart_K32L3A60_cm0plus)

include(driver_gpio_K32L3A60_cm0plus)

include(component_lpuart_adapter_K32L3A60_cm0plus)

include(component_lists_K32L3A60_cm0plus)

include(device_K32L3A60_CMSIS_K32L3A60_cm0plus)

include(CMSIS_Include_core_cm0plus_K32L3A60_cm0plus)

include(device_K32L3A60_startup_K32L3A60_cm0plus)

include(utilities_misc_utilities_K32L3A60_cm0plus)

include(CMSIS_Include_common_K32L3A60_cm0plus)

include(CMSIS_Include_dsp_K32L3A60_cm0plus)

include(device_K32L3A60_system_K32L3A60_cm0plus)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


