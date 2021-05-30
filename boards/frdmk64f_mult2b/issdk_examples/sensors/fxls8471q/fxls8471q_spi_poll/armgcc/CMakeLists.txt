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


project(fxls8471q_spi_poll_mult2b)

set(MCUX_SDK_PROJECT_NAME fxls8471q_spi_poll_mult2b.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../frdm_k64f.c"
"${ProjDirPath}/../../../../../frdm_k64f.h"
"${ProjDirPath}/../../../../../frdm_fxs_mult2b_shield.h"
"${ProjDirPath}/../../../../../issdk_hal.h"
"${ProjDirPath}/../../../../../RTE_Device.h"
"${ProjDirPath}/../fxls8471q_spi_poll.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../..
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../../../devices/MK64F12/drivers
    ${ProjDirPath}/../../../../../../../devices/MK64F12/cmsis_drivers
    ${ProjDirPath}/../../../../../../../devices/MK64F12/utilities
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/MK64F12
    ${ProjDirPath}/../../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../../CMSIS/Driver/Include
)

# include modules
include(middleware_issdk_drivers_gpio_kinetis_MK64F12)

include(driver_port_MK64F12)

include(driver_uart_MK64F12)

include(driver_gpio_MK64F12)

include(driver_i2c_MK64F12)

include(driver_i2c_edma_MK64F12)

include(driver_cmsis_i2c_MK64F12)

include(driver_dmamux_MK64F12)

include(driver_edma_MK64F12)

include(driver_dspi_MK64F12)

include(driver_dspi_edma_MK64F12)

include(driver_cmsis_dspi_MK64F12)

include(driver_smc_MK64F12)

include(driver_pit_MK64F12)

include(driver_uart_edma_MK64F12)

include(driver_cmsis_uart_MK64F12)

include(middleware_issdk_drivers_systick_utils_MK64F12)

include(middleware_issdk_sensor_interface_common_MK64F12)

include(middleware_issdk_sensor_fxls8471q_MK64F12)

include(driver_flash_MK64F12)

include(driver_clock_MK64F12)

include(utility_debug_console_MK64F12)

include(component_uart_adapter_MK64F12)

include(component_serial_manager_MK64F12)

include(driver_common_MK64F12)

include(component_lists_MK64F12)

include(component_serial_manager_uart_MK64F12)

include(device_MK64F12_CMSIS_MK64F12)

include(device_MK64F12_startup_MK64F12)

include(utility_assert_MK64F12)

include(CMSIS_Include_core_cm4_MK64F12)

include(CMSIS_Include_common_MK64F12)

include(CMSIS_Include_dsp_MK64F12)

include(CMSIS_Driver_Include_I2C_MK64F12)

include(CMSIS_Driver_Include_Common_MK64F12)

include(CMSIS_Driver_Include_SPI_MK64F12)

include(CMSIS_Driver_Include_USART_MK64F12)

include(utilities_misc_utilities_MK64F12)

include(device_MK64F12_system_MK64F12)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/K64F-MULT2B-FXLS8471Q-SPI-POLL.bin)

