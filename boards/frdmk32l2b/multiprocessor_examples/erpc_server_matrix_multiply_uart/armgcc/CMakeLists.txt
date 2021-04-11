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


project(erpc_server_matrix_multiply_uart)

set(MCUX_SDK_PROJECT_NAME erpc_server_matrix_multiply_uart.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../erpc_common/erpc_error_handler.h"
"${ProjDirPath}/../../erpc_common/erpc_error_handler.cpp"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.erpc"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.h"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.h"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.cpp"
"${ProjDirPath}/../main_server.c"
"${ProjDirPath}/../erpc_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../RTE_Device.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../erpc_common
    ${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/K32L2B31A/drivers
    ${ProjDirPath}/../../../../../devices/K32L2B31A/cmsis_drivers
    ${ProjDirPath}/../../../../../middleware/multicore
    ${ProjDirPath}/../../../../../devices/K32L2B31A
    ${ProjDirPath}/../../../../../devices/K32L2B31A/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(driver_lpuart_K32L2B31A)

include(driver_lpuart_dma_K32L2B31A)

include(driver_cmsis_lpuart_K32L2B31A)

include(driver_gpio_K32L2B31A)

include(driver_port_K32L2B31A)

include(driver_dma_K32L2B31A)

include(driver_dmamux_K32L2B31A)

include(middleware_multicore_erpc_eRPC_server_K32L2B31A)

include(middleware_multicore_erpc_eRPC_uart_cmsis_transport_K32L2B31A)

include(middleware_multicore_erpc_eRPC_port_stdlib_K32L2B31A)

include(middleware_multicore_erpc_doc_K32L2B31A)

include(driver_clock_K32L2B31A)

include(driver_common_K32L2B31A)

include(device_K32L2B31A_CMSIS_K32L2B31A)

include(utility_debug_console_K32L2B31A)

include(component_lpuart_adapter_K32L2B31A)

include(component_serial_manager_K32L2B31A)

include(component_lists_K32L2B31A)

include(component_serial_manager_uart_K32L2B31A)

include(device_K32L2B31A_startup_K32L2B31A)

include(driver_uart_K32L2B31A)

include(driver_smc_K32L2B31A)

include(utility_assert_K32L2B31A)

include(CMSIS_Include_core_cm0plus_K32L2B31A)

include(CMSIS_Include_common_K32L2B31A)

include(CMSIS_Include_dsp_K32L2B31A)

include(CMSIS_Driver_Include_USART_K32L2B31A)

include(CMSIS_Driver_Include_Common_K32L2B31A)

include(middleware_multicore_erpc_common_K32L2B31A)

include(utilities_misc_utilities_K32L2B31A)

include(device_K32L2B31A_system_K32L2B31A)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE g)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE stdc++)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


