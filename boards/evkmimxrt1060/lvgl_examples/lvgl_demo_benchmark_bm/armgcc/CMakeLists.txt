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


project(lvgl_demo_benchmark_bm)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME lvgl_demo_benchmark_bm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lvgl_demo_benchmark_bm.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/lv_demo_benchmark.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/lv_demo_benchmark.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/lv_demos.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_argb.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${SdkRootDirPath}/boards/evkmimxrt1060/lvgl_examples/lvgl_support.h"
"${SdkRootDirPath}/boards/evkmimxrt1060/lvgl_examples/lvgl_support.c"
"${SdkRootDirPath}/boards/evkmimxrt1060/lvgl_examples/lvgl_demo_utils.h"
"${SdkRootDirPath}/boards/evkmimxrt1060/lvgl_examples/lvgl_demo_utils.c"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos
    ${SdkRootDirPath}/middleware/lvgl/lvgl/examples/assets
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/stress/assets
    ${SdkRootDirPath}/boards/evkmimxrt1060/lvgl_examples
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1062/drivers
    ${SdkRootDirPath}/components/ft5406_rt
    ${SdkRootDirPath}/components/gt911
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware/lvgl
    ${SdkRootDirPath}/devices/MIMXRT1062
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1062/xip
    ${SdkRootDirPath}/boards/evkmimxrt1060/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_elcdif_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_ft5406_rt_MIMXRT1062)

include(driver_gt911_MIMXRT1062)

include(driver_video-common_MIMXRT1062)

include(driver_pxp_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

include(utility_debug_console_lite_MIMXRT1062)

include(utility_assert_lite_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(middleware_lvgl_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkmimxrt1060_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


