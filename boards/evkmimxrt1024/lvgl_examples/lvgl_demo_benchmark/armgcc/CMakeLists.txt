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
"${ProjDirPath}/../lv_demo_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/lv_demo.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets/img_cogwheel_alpha16.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets/img_cogwheel_rgb.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets/img_cogwheel_argb.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets/img_cogwheel_indexed16.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets/img_cogwheel_chroma_keyed.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_stress/assets/lv_font_montserrat_28_compr_az.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_stress/assets/lv_font_montserrat_16_compr_az.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_stress/assets/lv_font_montserrat_12_compr_az.c"
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
    ${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_benchmark
    ${ProjDirPath}/../../../../../middleware/lvgl/lv_examples
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets
    ${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_stress/assets
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/ili9341
    ${ProjDirPath}/../../../../../components/ft6x06
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/cmsis_drivers
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/lvgl
    ${ProjDirPath}/../../../../../devices/MIMXRT1024
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1024/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(driver_ili9341_MIMXRT1024)

include(driver_ft6x06_MIMXRT1024)

include(driver_clock_MIMXRT1024)

include(driver_lpuart_MIMXRT1024)

include(driver_cache_armv7_m7_MIMXRT1024)

include(driver_edma_MIMXRT1024)

include(driver_dmamux_MIMXRT1024)

include(driver_lpi2c_MIMXRT1024)

include(driver_lpi2c_edma_MIMXRT1024)

include(driver_lpspi_edma_MIMXRT1024)

include(driver_cmsis_lpspi_MIMXRT1024)

include(driver_cmsis_lpi2c_MIMXRT1024)

include(middleware_freertos-kernel_heap_4_MIMXRT1024)

include(driver_common_MIMXRT1024)

include(middleware_lvgl_MIMXRT1024)

include(device_MIMXRT1024_CMSIS_MIMXRT1024)

include(utility_debug_console_MIMXRT1024)

include(component_lpuart_adapter_MIMXRT1024)

include(component_serial_manager_MIMXRT1024)

include(component_lists_MIMXRT1024)

include(component_serial_manager_uart_MIMXRT1024)

include(device_MIMXRT1024_startup_MIMXRT1024)

include(driver_iomuxc_MIMXRT1024)

include(utility_assert_MIMXRT1024)

include(driver_igpio_MIMXRT1024)

include(driver_xip_device_MIMXRT1024)

include(driver_xip_board_evkmimxrt1024_MIMXRT1024)

include(CMSIS_Include_core_cm_MIMXRT1024)

include(CMSIS_Driver_Include_I2C_MIMXRT1024)

include(CMSIS_Driver_Include_Common_MIMXRT1024)

include(driver_lpspi_MIMXRT1024)

include(CMSIS_Driver_Include_SPI_MIMXRT1024)

include(middleware_freertos-kernel_MIMXRT1024)

include(middleware_freertos-kernel_extension_MIMXRT1024)

include(utilities_misc_utilities_MIMXRT1024)

include(device_MIMXRT1024_system_MIMXRT1024)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


