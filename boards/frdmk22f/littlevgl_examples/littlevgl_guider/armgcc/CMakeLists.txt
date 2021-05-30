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


project(littlevgl_guider)

set(MCUX_SDK_PROJECT_NAME littlevgl_guider.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../RTE_Device.h"
"${ProjDirPath}/../littlevgl_guider.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../generated/images/images.c"
"${ProjDirPath}/../generated/guider_fonts/guider_fonts.h"
"${ProjDirPath}/../generated/guider_customer_fonts/guider_customer_fonts.h"
"${ProjDirPath}/../generated/gui_guider.c"
"${ProjDirPath}/../generated/gui_guider.h"
"${ProjDirPath}/../generated/events_init.c"
"${ProjDirPath}/../generated/events_init.h"
"${ProjDirPath}/../custom/custom.c"
"${ProjDirPath}/../custom/custom.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../littlevgl_support.h"
"${ProjDirPath}/../../littlevgl_support.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/../..
    ${ProjDirPath}/..
    ${ProjDirPath}/../generated
    ${ProjDirPath}/../custom
    ${ProjDirPath}/../generated/guider_customer_fonts
    ${ProjDirPath}/../generated/guider_fonts
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/ili9341
    ${ProjDirPath}/../../../../../components/ft6x06
    ${ProjDirPath}/../../../../../devices/MK22F51212/drivers
    ${ProjDirPath}/../../../../../devices/MK22F51212/cmsis_drivers
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../middleware/littlevgl
    ${ProjDirPath}/../../../../../devices/MK22F51212
    ${ProjDirPath}/../../../../../devices/MK22F51212/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(driver_ili9341_MK22F51212)

include(driver_ft6x06_MK22F51212)

include(driver_port_MK22F51212)

include(driver_clock_MK22F51212)

include(driver_gpio_MK22F51212)

include(driver_lpuart_MK22F51212)

include(driver_edma_MK22F51212)

include(driver_dmamux_MK22F51212)

include(driver_dspi_MK22F51212)

include(driver_dspi_edma_MK22F51212)

include(driver_cmsis_dspi_MK22F51212)

include(driver_i2c_MK22F51212)

include(driver_i2c_edma_MK22F51212)

include(driver_cmsis_i2c_MK22F51212)

include(middleware_freertos-kernel_heap_4_MK22F51212)

include(driver_common_MK22F51212)

include(middleware_littlevgl_MK22F51212)

include(device_MK22F51212_CMSIS_MK22F51212)

include(utility_debug_console_MK22F51212)

include(component_uart_adapter_MK22F51212)

include(component_serial_manager_MK22F51212)

include(component_lists_MK22F51212)

include(component_serial_manager_uart_MK22F51212)

include(driver_uart_MK22F51212)

include(device_MK22F51212_startup_MK22F51212)

include(driver_smc_MK22F51212)

include(utility_assert_MK22F51212)

include(CMSIS_Include_core_cm4_MK22F51212)

include(CMSIS_Include_common_MK22F51212)

include(CMSIS_Include_dsp_MK22F51212)

include(CMSIS_Driver_Include_I2C_MK22F51212)

include(CMSIS_Driver_Include_Common_MK22F51212)

include(CMSIS_Driver_Include_SPI_MK22F51212)

include(middleware_freertos-kernel_MK22F51212)

include(middleware_freertos-kernel_extension_MK22F51212)

include(utilities_misc_utilities_MK22F51212)

include(device_MK22F51212_system_MK22F51212)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


