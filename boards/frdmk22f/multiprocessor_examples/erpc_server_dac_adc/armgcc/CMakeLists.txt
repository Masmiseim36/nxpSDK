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


project(erpc_server_dac_adc)

set(MCUX_SDK_PROJECT_NAME erpc_server_dac_adc.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../fxos.c"
"${ProjDirPath}/../../erpc_common/erpc_error_handler.h"
"${ProjDirPath}/../../erpc_common/erpc_error_handler.cpp"
"${ProjDirPath}/../main_server.c"
"${ProjDirPath}/../erpc_config.h"
"${ProjDirPath}/../service/erpc_dac_adc.erpc"
"${ProjDirPath}/../service/erpc_dac_adc.h"
"${ProjDirPath}/../service/erpc_dac_adc_server.h"
"${ProjDirPath}/../service/erpc_dac_adc_server.cpp"
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
    ${ProjDirPath}/..
    ${ProjDirPath}/../service
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MK22F51212/drivers
    ${ProjDirPath}/../../../../../components/fxos8700cq
    ${ProjDirPath}/../../../../../devices/MK22F51212/cmsis_drivers
    ${ProjDirPath}/../../../../../middleware/multicore
    ${ProjDirPath}/../../../../../devices/MK22F51212
    ${ProjDirPath}/../../../../../devices/MK22F51212/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(driver_uart_MK22F51212)

include(driver_uart_edma_MK22F51212)

include(driver_dac_MK22F51212)

include(driver_fxos8700cq_MK22F51212)

include(driver_cmsis_uart_MK22F51212)

include(driver_gpio_MK22F51212)

include(driver_port_MK22F51212)

include(driver_i2c_MK22F51212)

include(driver_adc16_MK22F51212)

include(driver_edma_MK22F51212)

include(driver_dmamux_MK22F51212)

include(middleware_multicore_erpc_eRPC_server_MK22F51212)

include(middleware_multicore_erpc_eRPC_uart_cmsis_transport_MK22F51212)

include(middleware_multicore_erpc_eRPC_port_stdlib_MK22F51212)

include(middleware_multicore_erpc_doc_MK22F51212)

include(driver_clock_MK22F51212)

include(driver_common_MK22F51212)

include(device_MK22F51212_CMSIS_MK22F51212)

include(utility_debug_console_MK22F51212)

include(component_uart_adapter_MK22F51212)

include(component_serial_manager_MK22F51212)

include(component_lists_MK22F51212)

include(component_serial_manager_uart_MK22F51212)

include(device_MK22F51212_startup_MK22F51212)

include(driver_lpuart_MK22F51212)

include(driver_smc_MK22F51212)

include(utility_assert_MK22F51212)

include(CMSIS_Include_core_cm4_MK22F51212)

include(CMSIS_Include_common_MK22F51212)

include(CMSIS_Include_dsp_MK22F51212)

include(CMSIS_Driver_Include_USART_MK22F51212)

include(CMSIS_Driver_Include_Common_MK22F51212)

include(middleware_multicore_erpc_common_MK22F51212)

include(utilities_misc_utilities_MK22F51212)

include(device_MK22F51212_system_MK22F51212)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE g)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE stdc++)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


