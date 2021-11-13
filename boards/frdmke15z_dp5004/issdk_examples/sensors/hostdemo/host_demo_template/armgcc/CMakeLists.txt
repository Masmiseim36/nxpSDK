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


project(hostdemo_mpxv5004dp)

set(MCUX_SDK_PROJECT_NAME hostdemo_mpxv5004dp.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../frdm_ke15z.c"
"${ProjDirPath}/../../../../../frdm_ke15z.h"
"${ProjDirPath}/../../../../../frdm-stbc-dp5004_shield.h"
"${ProjDirPath}/../../../../../issdk_hal.h"
"${ProjDirPath}/../../../../../RTE_Device.h"
"${ProjDirPath}/../host_demo.c"
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
    ${ProjDirPath}/../../../../../../../devices/MKE15Z7/drivers
    ${ProjDirPath}/../../../../../../../devices/MKE15Z7/cmsis_drivers
    ${ProjDirPath}/../../../../../../../devices/MKE15Z7
    ${ProjDirPath}/../../../../../../../devices/MKE15Z7/utilities
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../../CMSIS/Driver/Include
)

# include modules
include(middleware_issdk_drivers_gpio_kinetis_ke15z_MKE15Z7)

include(driver_port_MKE15Z7)

include(driver_adc12_MKE15Z7)

include(driver_lpi2c_MKE15Z7)

include(driver_lpi2c_edma_MKE15Z7)

include(driver_cmsis_lpi2c_MKE15Z7)

include(driver_lpspi_MKE15Z7)

include(driver_lpspi_edma_MKE15Z7)

include(driver_cmsis_lpspi_MKE15Z7)

include(driver_lpuart_MKE15Z7)

include(driver_lpuart_edma_MKE15Z7)

include(driver_cmsis_lpuart_MKE15Z7)

include(driver_dmamux_MKE15Z7)

include(driver_edma_MKE15Z7)

include(driver_trgmux_MKE15Z7)

include(driver_lptmr_MKE15Z7)

include(middleware_issdk_drivers_systick_utils_MKE15Z7)

include(middleware_issdk_sensor_interface_common_MKE15Z7)

include(middleware_issdk_sensor_allregdefs_MKE15Z7)

include(middleware_issdk_drivers_ads_MKE15Z7)

include(middleware_issdk_drivers_host_MKE15Z7)

include(driver_clock_MKE15Z7)

include(driver_flash_MKE15Z7)

include(driver_common_MKE15Z7)

include(device_MKE15Z7_CMSIS_MKE15Z7)

include(device_MKE15Z7_startup_MKE15Z7)

include(driver_smc_MKE15Z7)

include(utility_debug_console_MKE15Z7)

include(component_lpuart_adapter_MKE15Z7)

include(component_serial_manager_MKE15Z7)

include(component_lists_MKE15Z7)

include(component_serial_manager_uart_MKE15Z7)

include(driver_gpio_MKE15Z7)

include(utility_assert_MKE15Z7)

include(CMSIS_Include_core_cm_MKE15Z7)

include(CMSIS_Driver_Include_I2C_MKE15Z7)

include(CMSIS_Driver_Include_Common_MKE15Z7)

include(CMSIS_Driver_Include_SPI_MKE15Z7)

include(CMSIS_Driver_Include_USART_MKE15Z7)

include(utilities_misc_utilities_MKE15Z7)

include(device_MKE15Z7_system_MKE15Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/KE15Z-DP5004-HOST-DEMO.bin)

