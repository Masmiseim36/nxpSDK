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


project(fxas21002_fifo)

set(MCUX_SDK_PROJECT_NAME fxas21002_fifo.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../../frdm_k32l3a6.c"
"${ProjDirPath}/../../../../../../frdm_k32l3a6.h"
"${ProjDirPath}/../../../../../../frdm_stbc_agm01_shield.h"
"${ProjDirPath}/../../../../../../issdk_hal.h"
"${ProjDirPath}/../../../../../../RTE_Device.h"
"${ProjDirPath}/../fxas21002_fifo.c"
"${ProjDirPath}/../../pin_mux.c"
"${ProjDirPath}/../../pin_mux.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../..
    ${ProjDirPath}/../..
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities
    ${ProjDirPath}/../../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../../devices/K32L3A60
    ${ProjDirPath}/../../../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../../../devices
    ${ProjDirPath}/../../../../../../../../CMSIS/Driver/Include
)

# include modules
include(middleware_issdk_drivers_gpio_kinetis_K32L3A60_cm4)

include(driver_port_K32L3A60_cm4)

include(driver_clock_K32L3A60_cm4)

include(driver_gpio_K32L3A60_cm4)

include(driver_edma_K32L3A60_cm4)

include(driver_dmamux_K32L3A60_cm4)

include(driver_lpuart_K32L3A60_cm4)

include(driver_lpuart_edma_K32L3A60_cm4)

include(driver_cmsis_lpuart_K32L3A60_cm4)

include(driver_lpi2c_K32L3A60_cm4)

include(driver_lpi2c_edma_K32L3A60_cm4)

include(driver_cmsis_lpi2c_K32L3A60_cm4)

include(driver_lpspi_K32L3A60_cm4)

include(driver_lpspi_edma_K32L3A60_cm4)

include(driver_cmsis_lpspi_K32L3A60_cm4)

include(middleware_issdk_drivers_systick_utils_K32L3A60_cm4)

include(middleware_issdk_sensor_interface_common_K32L3A60_cm4)

include(middleware_issdk_sensor_fxas21002_K32L3A60_cm4)

include(driver_common_K32L3A60_cm4)

include(driver_msmc_K32L3A60_cm4)

include(utility_assert_K32L3A60_cm4)

include(utility_debug_console_K32L3A60_cm4)

include(component_lpuart_adapter_K32L3A60_cm4)

include(component_serial_manager_K32L3A60_cm4)

include(component_lists_K32L3A60_cm4)

include(component_serial_manager_uart_K32L3A60_cm4)

include(device_K32L3A60_CMSIS_K32L3A60_cm4)

include(CMSIS_Include_core_cm4_K32L3A60_cm4)

include(device_K32L3A60_startup_K32L3A60_cm4)

include(CMSIS_Driver_Include_USART_K32L3A60_cm4)

include(CMSIS_Driver_Include_Common_K32L3A60_cm4)

include(CMSIS_Driver_Include_I2C_K32L3A60_cm4)

include(CMSIS_Driver_Include_SPI_K32L3A60_cm4)

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

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/FRDM-K32L3A6-AGM01-FXAS21002-FIFO.bin)

