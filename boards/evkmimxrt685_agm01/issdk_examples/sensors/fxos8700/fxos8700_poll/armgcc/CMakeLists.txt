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


project(fxos8700_poll)

set(MCUX_SDK_PROJECT_NAME fxos8700_poll.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../evkmimxrt685.c"
"${ProjDirPath}/../../../../../evkmimxrt685.h"
"${ProjDirPath}/../../../../../frdm_stbc_agm01_shield.h"
"${ProjDirPath}/../../../../../issdk_hal.h"
"${ProjDirPath}/../../../../../RTE_Device.h"
"${ProjDirPath}/../fxos8700_poll.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../..
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../../../devices/MIMXRT685S/drivers
    ${ProjDirPath}/../../../../../../../devices/MIMXRT685S/cmsis_drivers
    ${ProjDirPath}/../../../../../../../devices/MIMXRT685S
    ${ProjDirPath}/../../../../../../../devices/MIMXRT685S/utilities
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../evkmimxrt685/flash_config
    ${ProjDirPath}/../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../../CMSIS/Driver/Include
)

# include modules
include(middleware_issdk_drivers_gpio_imxrt600_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_inputmux_MIMXRT685S_cm33)

include(driver_inputmux_connections_MIMXRT685S_cm33)

include(driver_lpc_dma_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_dma_MIMXRT685S_cm33)

include(driver_cmsis_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_flexcomm_spi_MIMXRT685S_cm33)

include(driver_flexcomm_spi_dma_MIMXRT685S_cm33)

include(driver_cmsis_flexcomm_spi_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(driver_flexcomm_usart_dma_MIMXRT685S_cm33)

include(driver_cmsis_flexcomm_usart_MIMXRT685S_cm33)

include(middleware_issdk_drivers_systick_utils_evkmimxrt685_MIMXRT685S_cm33)

include(middleware_issdk_sensor_interface_common_MIMXRT685S_cm33)

include(middleware_issdk_sensor_fxos8700_MIMXRT685S_cm33)

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

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm33_MIMXRT685S_cm33)

include(CMSIS_Include_common_MIMXRT685S_cm33)

include(CMSIS_Include_dsp_MIMXRT685S_cm33)

include(CMSIS_Driver_Include_I2C_MIMXRT685S_cm33)

include(CMSIS_Driver_Include_Common_MIMXRT685S_cm33)

include(CMSIS_Driver_Include_SPI_MIMXRT685S_cm33)

include(CMSIS_Driver_Include_USART_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/EVKMIMXRT685-AGM01-FXOS8700-POLL.bin)

