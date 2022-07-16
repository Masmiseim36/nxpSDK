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


project(lvgl_demo_benchmark)

set(MCUX_SDK_PROJECT_NAME lvgl_demo_benchmark.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../RTE_Device.h"
"${ProjDirPath}/../lvgl_demo_benchmark.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/lv_demo_benchmark.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/lv_demo_benchmark.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/lv_demos.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_argb.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../lvgl_support.h"
"${ProjDirPath}/../../lvgl_support.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../..
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/stress/assets
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/ili9341
    ${ProjDirPath}/../../../../../components/ft6x06
    ${ProjDirPath}/../../../../../devices/MIMXRT1015/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1015/cmsis_drivers
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/lvgl
    ${ProjDirPath}/../../../../../devices/MIMXRT1015
    ${ProjDirPath}/../../../../../devices/MIMXRT1015/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1015/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(driver_ili9341_MIMXRT1015)

include(driver_ft6x06_MIMXRT1015)

include(driver_clock_MIMXRT1015)

include(driver_lpuart_MIMXRT1015)

include(driver_cache_armv7_m7_MIMXRT1015)

include(driver_edma_MIMXRT1015)

include(driver_dmamux_MIMXRT1015)

include(driver_lpi2c_MIMXRT1015)

include(driver_lpi2c_edma_MIMXRT1015)

include(driver_lpspi_edma_MIMXRT1015)

include(driver_cmsis_lpspi_MIMXRT1015)

include(driver_cmsis_lpi2c_MIMXRT1015)

include(middleware_freertos-kernel_heap_4_MIMXRT1015)

include(driver_common_MIMXRT1015)

include(middleware_lvgl_MIMXRT1015)

include(device_MIMXRT1015_CMSIS_MIMXRT1015)

include(utility_debug_console_MIMXRT1015)

include(component_lpuart_adapter_MIMXRT1015)

include(component_serial_manager_MIMXRT1015)

include(component_lists_MIMXRT1015)

include(component_serial_manager_uart_MIMXRT1015)

include(device_MIMXRT1015_startup_MIMXRT1015)

include(driver_iomuxc_MIMXRT1015)

include(utility_assert_MIMXRT1015)

include(driver_igpio_MIMXRT1015)

include(driver_xip_device_MIMXRT1015)

include(driver_xip_board_evkmimxrt1015_MIMXRT1015)

include(CMSIS_Include_core_cm_MIMXRT1015)

include(CMSIS_Driver_Include_I2C_MIMXRT1015)

include(CMSIS_Driver_Include_Common_MIMXRT1015)

include(driver_lpspi_MIMXRT1015)

include(CMSIS_Driver_Include_SPI_MIMXRT1015)

include(middleware_freertos-kernel_MIMXRT1015)

include(middleware_freertos-kernel_extension_MIMXRT1015)

include(utilities_misc_utilities_MIMXRT1015)

include(device_MIMXRT1015_system_MIMXRT1015)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


