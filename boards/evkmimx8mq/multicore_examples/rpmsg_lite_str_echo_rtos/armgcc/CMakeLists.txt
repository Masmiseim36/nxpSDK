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


project(rpmsg_lite_str_echo_rtos_imxcm4)

set(MCUX_SDK_PROJECT_NAME rpmsg_lite_str_echo_rtos_imxcm4.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main_remote.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../rsc_table.c"
"${ProjDirPath}/../rsc_table.h"
"${ProjDirPath}/../remoteproc.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../fsl_iomuxc.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../middleware/multicore
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../devices/MIMX8MQ6/drivers
    ${ProjDirPath}/../../../../../devices/MIMX8MQ6
    ${ProjDirPath}/../../../../../devices/MIMX8MQ6/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(middleware_multicore_rpmsg_lite_imx8mq_m4_freertos_MIMX8MQ6)

include(middleware_freertos-kernel_heap_4_MIMX8MQ6)

include(driver_clock_MIMX8MQ6)

include(driver_mu_MIMX8MQ6)

include(middleware_freertos-kernel_MIMX8MQ6)

include(driver_common_MIMX8MQ6)

include(driver_rdc_MIMX8MQ6)

include(device_MIMX8MQ6_CMSIS_MIMX8MQ6)

include(utility_debug_console_MIMX8MQ6)

include(component_iuart_adapter_MIMX8MQ6)

include(component_serial_manager_MIMX8MQ6)

include(component_lists_MIMX8MQ6)

include(component_serial_manager_uart_MIMX8MQ6)

include(driver_iuart_MIMX8MQ6)

include(device_MIMX8MQ6_startup_MIMX8MQ6)

include(utility_assert_MIMX8MQ6)

include(middleware_multicore_rpmsg_lite_MIMX8MQ6)

include(middleware_freertos-kernel_extension_MIMX8MQ6)

include(CMSIS_Include_core_cm4_MIMX8MQ6)

include(CMSIS_Include_common_MIMX8MQ6)

include(CMSIS_Include_dsp_MIMX8MQ6)

include(utilities_misc_utilities_MIMX8MQ6)

include(device_MIMX8MQ6_system_MIMX8MQ6)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/rpmsg_lite_str_echo_rtos.bin)

