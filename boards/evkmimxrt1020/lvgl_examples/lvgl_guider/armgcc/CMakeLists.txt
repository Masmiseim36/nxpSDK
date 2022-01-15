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


project(lvgl_guider)

set(MCUX_SDK_PROJECT_NAME lvgl_guider.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../RTE_Device.h"
"${ProjDirPath}/../lvgl_guider.c"
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
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../lvgl_support.h"
"${ProjDirPath}/../../lvgl_support.c"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../..
    ${ProjDirPath}/../generated
    ${ProjDirPath}/../custom
    ${ProjDirPath}/../generated/guider_customer_fonts
    ${ProjDirPath}/../generated/guider_fonts
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/ili9341
    ${ProjDirPath}/../../../../../components/ft6x06
    ${ProjDirPath}/../../../../../devices/MIMXRT1021/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1021/cmsis_drivers
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/lvgl
    ${ProjDirPath}/../../../../../devices/MIMXRT1021
    ${ProjDirPath}/../../../../../devices/MIMXRT1021/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1021/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(driver_ili9341_MIMXRT1021)

include(driver_ft6x06_MIMXRT1021)

include(driver_clock_MIMXRT1021)

include(driver_lpuart_MIMXRT1021)

include(driver_cache_armv7_m7_MIMXRT1021)

include(driver_edma_MIMXRT1021)

include(driver_dmamux_MIMXRT1021)

include(driver_lpi2c_MIMXRT1021)

include(driver_lpi2c_edma_MIMXRT1021)

include(driver_lpspi_edma_MIMXRT1021)

include(driver_cmsis_lpspi_MIMXRT1021)

include(driver_cmsis_lpi2c_MIMXRT1021)

include(middleware_freertos-kernel_heap_4_MIMXRT1021)

include(driver_common_MIMXRT1021)

include(middleware_lvgl_MIMXRT1021)

include(device_MIMXRT1021_CMSIS_MIMXRT1021)

include(utility_debug_console_MIMXRT1021)

include(component_lpuart_adapter_MIMXRT1021)

include(component_serial_manager_MIMXRT1021)

include(component_lists_MIMXRT1021)

include(component_serial_manager_uart_MIMXRT1021)

include(device_MIMXRT1021_startup_MIMXRT1021)

include(driver_iomuxc_MIMXRT1021)

include(utility_assert_MIMXRT1021)

include(driver_igpio_MIMXRT1021)

include(driver_xip_device_MIMXRT1021)

include(driver_xip_board_evkmimxrt1020_MIMXRT1021)

include(CMSIS_Include_core_cm_MIMXRT1021)

include(CMSIS_Driver_Include_I2C_MIMXRT1021)

include(CMSIS_Driver_Include_Common_MIMXRT1021)

include(driver_lpspi_MIMXRT1021)

include(CMSIS_Driver_Include_SPI_MIMXRT1021)

include(middleware_freertos-kernel_MIMXRT1021)

include(middleware_freertos-kernel_extension_MIMXRT1021)

include(utilities_misc_utilities_MIMXRT1021)

include(device_MIMXRT1021_system_MIMXRT1021)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


