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


project(lvgl_demo_widgets_bm)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME lvgl_demo_widgets_bm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lvgl_demo_widgets_bm.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/lv_demo_widgets.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/lv_demo_widgets.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets/img_clothes.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets/img_demo_widgets_avatar.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets/img_lvgl_logo.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/lv_demos.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${SdkRootDirPath}/boards/evkmimxrt1064/lvgl_examples/lvgl_support.h"
"${SdkRootDirPath}/boards/evkmimxrt1064/lvgl_examples/lvgl_support.c"
"${SdkRootDirPath}/boards/evkmimxrt1064/lvgl_examples/lvgl_demo_utils.h"
"${SdkRootDirPath}/boards/evkmimxrt1064/lvgl_examples/lvgl_demo_utils.c"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos
    ${SdkRootDirPath}/boards/evkmimxrt1064/lvgl_examples
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1064/drivers
    ${SdkRootDirPath}/components/ft5406_rt
    ${SdkRootDirPath}/devices/MIMXRT1064/utilities
    ${SdkRootDirPath}/devices/MIMXRT1064/utilities/debug_console_lite
    ${SdkRootDirPath}/components/gt911
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/middleware/lvgl
    ${SdkRootDirPath}/devices/MIMXRT1064
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1064/xip
    ${SdkRootDirPath}/boards/evkmimxrt1064/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_elcdif_MIMXRT1064)

include(driver_lpi2c_MIMXRT1064)

include(driver_ft5406_rt_MIMXRT1064)

include(driver_pxp_MIMXRT1064)

include(driver_cache_armv7_m7_MIMXRT1064)

include(utility_debug_console_lite_MIMXRT1064)

include(utility_assert_lite_MIMXRT1064)

include(driver_gt911_MIMXRT1064)

include(driver_video-common_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(middleware_lvgl_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_evkmimxrt1064_MIMXRT1064)

include(CMSIS_Include_core_cm_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


