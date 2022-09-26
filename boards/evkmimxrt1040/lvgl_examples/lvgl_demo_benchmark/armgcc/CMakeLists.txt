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
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/benchmark
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/examples/assets
    ${ProjDirPath}/../../../../../middleware/lvgl/lvgl/demos/stress/assets
    ${ProjDirPath}/../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1042/drivers
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../components/gt911
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/lvgl
    ${ProjDirPath}/../../../../../devices/MIMXRT1042
    ${ProjDirPath}/../../../../../devices/MIMXRT1042/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1042/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../components/silicon_id
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
)

# include modules
include(driver_elcdif_MIMXRT1042)

include(driver_lpi2c_MIMXRT1042)

include(driver_ft5406_rt_MIMXRT1042)

include(driver_pxp_MIMXRT1042)

include(driver_cache_armv7_m7_MIMXRT1042)

include(driver_gt911_MIMXRT1042)

include(driver_video-common_MIMXRT1042)

include(middleware_freertos-kernel_heap_4_MIMXRT1042)

include(driver_common_MIMXRT1042)

include(driver_clock_MIMXRT1042)

include(middleware_lvgl_MIMXRT1042)

include(device_MIMXRT1042_CMSIS_MIMXRT1042)

include(utility_debug_console_MIMXRT1042)

include(component_lpuart_adapter_MIMXRT1042)

include(component_serial_manager_MIMXRT1042)

include(component_lists_MIMXRT1042)

include(component_serial_manager_uart_MIMXRT1042)

include(driver_lpuart_MIMXRT1042)

include(device_MIMXRT1042_startup_MIMXRT1042)

include(driver_iomuxc_MIMXRT1042)

include(utility_assert_MIMXRT1042)

include(driver_igpio_MIMXRT1042)

include(driver_xip_device_MIMXRT1042)

include(driver_xip_board_evkmimxrt1040_MIMXRT1042)

include(component_silicon_id_MIMXRT1042)

include(CMSIS_Include_core_cm_MIMXRT1042)

include(middleware_freertos-kernel_MIMXRT1042)

include(middleware_freertos-kernel_extension_MIMXRT1042)

include(utilities_misc_utilities_MIMXRT1042)

include(device_MIMXRT1042_system_MIMXRT1042)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


