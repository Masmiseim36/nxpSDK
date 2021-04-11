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


project(uart_interrupt_rb_transfer)

set(MCUX_SDK_PROJECT_NAME uart_interrupt_rb_transfer.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../uart_interrupt_rb_transfer.c"
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
    ${ProjDirPath}/../../../../../../devices/K32L2B31A/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../devices/K32L2B31A/drivers
    ${ProjDirPath}/../../../../../../devices/K32L2B31A
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../devices/K32L2B31A/utilities
)

# include modules
include(utility_debug_console_lite_K32L2B31A)

include(utility_assert_lite_K32L2B31A)

include(driver_port_K32L2B31A)

include(driver_uart_K32L2B31A)

include(driver_clock_K32L2B31A)

include(driver_common_K32L2B31A)

include(device_K32L2B31A_CMSIS_K32L2B31A)

include(component_lpuart_adapter_K32L2B31A)

include(component_lists_K32L2B31A)

include(driver_lpuart_K32L2B31A)

include(device_K32L2B31A_startup_K32L2B31A)

include(driver_smc_K32L2B31A)

include(driver_gpio_K32L2B31A)

include(CMSIS_Include_core_cm0plus_K32L2B31A)

include(CMSIS_Include_common_K32L2B31A)

include(CMSIS_Include_dsp_K32L2B31A)

include(utilities_misc_utilities_K32L2B31A)

include(device_K32L2B31A_system_K32L2B31A)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


