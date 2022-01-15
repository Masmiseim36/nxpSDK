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


project(csi_mipi_rgb_cm7)

set(MCUX_SDK_PROJECT_NAME csi_mipi_rgb_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../csi_mipi_rgb.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../camera_support.h"
"${ProjDirPath}/../camera_support.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm7side.jlinkscript"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../../../components/video/camera/device/ov5640
    ${ProjDirPath}/../../../../../../../components/video/display/dc/lcdifv2
    ${ProjDirPath}/../../../../../../../components/video/display
    ${ProjDirPath}/../../../../../../../components/video/display/rm68191
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../../components/video/display/mipi_dsi_cmd
    ${ProjDirPath}/../../../../../../../components/video/display/rm68200
    ${ProjDirPath}/../../../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../../components/video/display/hx8394
    ${ProjDirPath}/../../../../../../../components/video/camera/receiver/csi
    ${ProjDirPath}/../../../../../../../components/video/camera
    ${ProjDirPath}/../../../../../../../components/video
    ${ProjDirPath}/../../../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../../../components/video/camera/receiver
    ${ProjDirPath}/../../../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../../xip
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/drivers/cm7
    ${ProjDirPath}/../../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1176/utilities
)

# include modules
include(driver_camera-device-sccb_MIMXRT1176_cm7)

include(driver_camera-device-ov5640_MIMXRT1176_cm7)

include(driver_dc-fb-lcdifv2_MIMXRT1176_cm7)

include(driver_display-common_MIMXRT1176_cm7)

include(driver_display-rm68191_MIMXRT1176_cm7)

include(driver_mipi_dsi_split_MIMXRT1176_cm7)

include(driver_display-mipi-dsi-cmd_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(driver_display-rm68200_MIMXRT1176_cm7)

include(driver_soc_mipi_csi2rx_MIMXRT1176_cm7)

include(driver_soc_src_MIMXRT1176_cm7)

include(driver_elcdif_MIMXRT1176_cm7)

include(driver_dc-fb-elcdif_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(driver_display-hx8394_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_csi_MIMXRT1176_cm7)

include(driver_camera-receiver-csi_MIMXRT1176_cm7)

include(driver_camera-common_MIMXRT1176_cm7)

include(driver_video-common_MIMXRT1176_cm7)

include(driver_camera-device-common_MIMXRT1176_cm7)

include(driver_camera-receiver-common_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_dc-fb-common_MIMXRT1176_cm7)

include(driver_mipi_csi2rx_MIMXRT1176_cm7)

include(driver_pxp_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_cache_armv7_m7_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(driver_lcdifv2_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


