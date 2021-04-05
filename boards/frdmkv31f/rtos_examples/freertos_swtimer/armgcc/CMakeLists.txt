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


project(freertos_swtimer)

set(MCUX_SDK_PROJECT_NAME freertos_swtimer.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../freertos_swtimer.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MKV31F51212/drivers
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../devices/MKV31F51212
    ${ProjDirPath}/../../../../../devices/MKV31F51212/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(driver_port_MKV31F51212)

include(driver_gpio_MKV31F51212)

include(driver_clock_MKV31F51212)

include(middleware_freertos-kernel_heap_4_MKV31F51212)

include(driver_common_MKV31F51212)

include(device_MKV31F51212_CMSIS_MKV31F51212)

include(utility_debug_console_MKV31F51212)

include(component_uart_adapter_MKV31F51212)

include(component_serial_manager_MKV31F51212)

include(component_lists_MKV31F51212)

include(component_serial_manager_uart_MKV31F51212)

include(driver_uart_MKV31F51212)

include(device_MKV31F51212_startup_MKV31F51212)

include(driver_lpuart_MKV31F51212)

include(driver_smc_MKV31F51212)

include(utility_assert_MKV31F51212)

include(CMSIS_Include_core_cm4_MKV31F51212)

include(CMSIS_Include_common_MKV31F51212)

include(CMSIS_Include_dsp_MKV31F51212)

include(middleware_freertos-kernel_MKV31F51212)

include(middleware_freertos-kernel_extension_MKV31F51212)

include(utilities_misc_utilities_MKV31F51212)

include(device_MKV31F51212_system_MKV31F51212)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


