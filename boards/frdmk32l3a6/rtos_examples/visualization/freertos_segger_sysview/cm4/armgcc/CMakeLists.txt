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


project(freertos_segger_sysview_cm4)

set(MCUX_SDK_PROJECT_NAME freertos_segger_sysview_cm4.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../../SEGGER/cm4/SEGGER.h"
"${ProjDirPath}/../../SEGGER/cm4/SEGGER_RTT.c"
"${ProjDirPath}/../../SEGGER/cm4/SEGGER_RTT.h"
"${ProjDirPath}/../../SEGGER/cm4/SEGGER_SYSVIEW.c"
"${ProjDirPath}/../../SEGGER/cm4/SEGGER_SYSVIEW.h"
"${ProjDirPath}/../../SEGGER/cm4/SEGGER_SYSVIEW_ConfDefaults.h"
"${ProjDirPath}/../../SEGGER/cm4/SEGGER_SYSVIEW_Int.h"
"${ProjDirPath}/../../Config/cm4/Global.h"
"${ProjDirPath}/../../Config/cm4/SEGGER_RTT_Conf.h"
"${ProjDirPath}/../../Config/cm4/SEGGER_SYSVIEW_Conf.h"
"${ProjDirPath}/../../Sample/OS/cm4/SEGGER_SYSVIEW_FreeRTOS.c"
"${ProjDirPath}/../../Sample/OS/cm4/SEGGER_SYSVIEW_FreeRTOS.h"
"${ProjDirPath}/../../Sample/OS/cm4/pin_mux.c"
"${ProjDirPath}/../../Sample/OS/cm4/pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/..
    ${ProjDirPath}/../../SEGGER/cm4
    ${ProjDirPath}/../../Config/cm4
    ${ProjDirPath}/../../Sample/OS/cm4
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../devices/K32L3A60/drivers
    ${ProjDirPath}/../../../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../../../devices/K32L3A60/utilities
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/K32L3A60
    ${ProjDirPath}/../../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../../devices
)

# include modules
include(driver_port_K32L3A60_cm4)

include(driver_gpio_K32L3A60_cm4)

include(driver_clock_K32L3A60_cm4)

include(middleware_freertos-kernel_heap_4_K32L3A60_cm4)

include(driver_common_K32L3A60_cm4)

include(driver_msmc_K32L3A60_cm4)

include(driver_lpuart_K32L3A60_cm4)

include(utility_assert_K32L3A60_cm4)

include(utility_debug_console_K32L3A60_cm4)

include(component_lpuart_adapter_K32L3A60_cm4)

include(component_serial_manager_K32L3A60_cm4)

include(component_lists_K32L3A60_cm4)

include(component_serial_manager_uart_K32L3A60_cm4)

include(device_K32L3A60_CMSIS_K32L3A60_cm4)

include(CMSIS_Include_core_cm4_K32L3A60_cm4)

include(device_K32L3A60_startup_K32L3A60_cm4)

include(middleware_freertos-kernel_K32L3A60_cm4)

include(middleware_freertos-kernel_extension_K32L3A60_cm4)

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


