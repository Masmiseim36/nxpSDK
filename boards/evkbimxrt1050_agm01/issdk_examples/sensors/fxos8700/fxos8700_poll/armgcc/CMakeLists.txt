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


project(fxos8700_poll)

set(MCUX_SDK_PROJECT_NAME fxos8700_poll.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../evkbimxrt1050.c"
"${ProjDirPath}/../../../../../evkbimxrt1050.h"
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
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../..
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1052/drivers
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1052/cmsis_drivers
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1052
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1052/utilities
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1052/xip
    ${ProjDirPath}/../../../../../../evkbimxrt1050/xip
    ${ProjDirPath}/../../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../../CMSIS/Driver/Include
)

# include modules
include(middleware_issdk_drivers_gpio_imx_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(driver_edma_MIMXRT1052)

include(driver_dmamux_MIMXRT1052)

include(driver_lpi2c_MIMXRT1052)

include(driver_lpi2c_edma_MIMXRT1052)

include(driver_cmsis_lpi2c_MIMXRT1052)

include(driver_lpspi_MIMXRT1052)

include(driver_lpspi_edma_MIMXRT1052)

include(driver_cmsis_lpspi_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(driver_lpuart_edma_MIMXRT1052)

include(driver_cmsis_lpuart_MIMXRT1052)

include(middleware_issdk_drivers_systick_utils_evkbimxrt1050_MIMXRT1052)

include(middleware_issdk_sensor_interface_common_MIMXRT1052)

include(middleware_issdk_sensor_fxos8700_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(utility_debug_console_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(component_serial_manager_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(component_serial_manager_uart_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(utility_assert_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_evkbimxrt1050_MIMXRT1052)

include(CMSIS_Include_core_cm_MIMXRT1052)

include(CMSIS_Driver_Include_I2C_MIMXRT1052)

include(CMSIS_Driver_Include_Common_MIMXRT1052)

include(CMSIS_Driver_Include_SPI_MIMXRT1052)

include(CMSIS_Driver_Include_USART_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/EVKBIMXRT1050-AGM01-FXOS8700-POLL.bin)

