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


project(freertos_blinky_ns)

set(MCUX_SDK_PROJECT_NAME freertos_blinky_ns.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main_ns.c"
"${ProjDirPath}/../../freertos_blinky_s/nsc_functions.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33/secure/secure_context.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33/secure/secure_init.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../freertos_blinky_s
    ${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33/secure
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers
    ${ProjDirPath}/../../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../../devices/MIMXRT685S
    ${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../flash_config
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
)

# include modules
include(driver_clock_MIMXRT685S_cm33)

include(middleware_freertos-kernel_cm33_secure_port_MIMXRT685S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(utility_debug_console_MIMXRT685S_cm33)

include(utility_assert_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_flash_config_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(component_serial_manager_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_serial_manager_uart_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(middleware_freertos-kernel_MIMXRT685S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT685S_cm33)

include(middleware_freertos-kernel_mpu_wrappers_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../freertos_blinky_s/armgcc/${CMAKE_BUILD_TYPE}/freertos_blinky_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../freertos_blinky_s/armgcc/${CMAKE_BUILD_TYPE}/freertos_blinky_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL flash_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../freertos_blinky_s/armgcc/${CMAKE_BUILD_TYPE}/freertos_blinky_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL flash_debug)

if(CMAKE_BUILD_TYPE STREQUAL flash_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../freertos_blinky_s/armgcc/${CMAKE_BUILD_TYPE}/freertos_blinky_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL flash_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

