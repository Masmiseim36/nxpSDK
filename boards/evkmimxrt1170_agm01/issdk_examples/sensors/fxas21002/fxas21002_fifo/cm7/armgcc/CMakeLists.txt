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


project(fxas21002_fifo)

set(MCUX_SDK_PROJECT_NAME fxas21002_fifo.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../../evkmimxrt1170.c"
"${ProjDirPath}/../../../../../../evkmimxrt1170.h"
"${ProjDirPath}/../../../../../../frdm_stbc_agm01_shield.h"
"${ProjDirPath}/../../../../../../issdk_hal.h"
"${ProjDirPath}/../../../../../../RTE_Device.h"
"${ProjDirPath}/../fxas21002_fifo.c"
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
    ${ProjDirPath}/../../../../../..
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../../../devices/MIMXRT1176/cmsis_drivers
    ${ProjDirPath}/../../../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../../../devices/MIMXRT1176/utilities
    ${ProjDirPath}/../../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../../../../evkmimxrt1170/xip
    ${ProjDirPath}/../../../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../../../CMSIS/Driver/Include
)

# include modules
include(middleware_issdk_drivers_gpio_imx_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_edma_MIMXRT1176_cm7)

include(driver_dmamux_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_lpi2c_edma_MIMXRT1176_cm7)

include(driver_cmsis_lpi2c_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_lpspi_MIMXRT1176_cm7)

include(driver_lpspi_edma_MIMXRT1176_cm7)

include(driver_cmsis_lpspi_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(driver_lpuart_edma_MIMXRT1176_cm7)

include(driver_cmsis_lpuart_MIMXRT1176_cm7)

include(middleware_issdk_drivers_systick_utils_evkmimxrt1170_MIMXRT1176_cm7)

include(middleware_issdk_sensor_interface_common_MIMXRT1176_cm7)

include(middleware_issdk_sensor_fxas21002_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(utility_debug_console_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_serial_manager_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_serial_manager_uart_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(utility_assert_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_gpc_3_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_I2C_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_Common_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_SPI_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_USART_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/EVKMIMXRT1170-AGM01-FXAS21002-FIFO.bin)

