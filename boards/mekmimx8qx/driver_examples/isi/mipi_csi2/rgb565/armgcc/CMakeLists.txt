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


project(isi_mipi_csi2_rgb565)

set(MCUX_SDK_PROJECT_NAME isi_mipi_csi2_rgb565.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../isi_mipi_csi2_rgb565.c"
"${ProjDirPath}/../app_display.h"
"${ProjDirPath}/../app_display.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../isi_board.c"
"${ProjDirPath}/../isi_config.h"
"${ProjDirPath}/../isi_example.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../empty_rsc_table.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../devices/MIMX8QX6/drivers
    ${ProjDirPath}/../../../../../../../components/video
    ${ProjDirPath}/../../../../../../../components/video/i2c
    ${ProjDirPath}/../../../../../../../components/video/display
    ${ProjDirPath}/../../../../../../../components/video/display/adv7535
    ${ProjDirPath}/../../../../../../../components/video/display/rm67191
    ${ProjDirPath}/../../../../../../../components/video/display/mipi_dsi_cmd
    ${ProjDirPath}/../../../../../../../components/video/camera/device/ov5640
    ${ProjDirPath}/../../../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../../../components/video/display/it6263
    ${ProjDirPath}/../../../../../../../devices/MIMX8QX6/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../../components/video/camera
    ${ProjDirPath}/../../../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../../../components/video/camera/receiver
    ${ProjDirPath}/../../../../../../../components/video/camera/receiver/isi
    ${ProjDirPath}/../../../../../../../devices/MIMX8QX6
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/MIMX8QX6/scfw_api
    ${ProjDirPath}/../../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../../devices/MIMX8QX6/utilities
)

# include modules
include(driver_irqsteer_MIMX8QX6)

include(driver_mipi_dsi_MIMX8QX6)

include(driver_lpi2c_MIMX8QX6)

include(driver_video-common_MIMX8QX6)

include(driver_dpu_MIMX8QX6)

include(driver_video-i2c_MIMX8QX6)

include(driver_display-common_MIMX8QX6)

include(driver_display-adv7535_MIMX8QX6)

include(driver_display-rm67191_MIMX8QX6)

include(driver_display-mipi-dsi-cmd_MIMX8QX6)

include(driver_dpu_irqsteer_MIMX8QX6)

include(driver_camera-device-ov5640_MIMX8QX6)

include(driver_camera-device-sccb_MIMX8QX6)

include(driver_display-it6263_MIMX8QX6)

include(driver_ldb_combo_phy_MIMX8QX6)

include(driver_prg_MIMX8QX6)

include(driver_dpr_MIMX8QX6)

include(utility_debug_console_lite_MIMX8QX6)

include(utility_assert_lite_MIMX8QX6)

include(driver_igpio_MIMX8QX6)

include(driver_isi_MIMX8QX6)

include(driver_mipi_csi2rx_MIMX8QX6)

include(driver_camera-common_MIMX8QX6)

include(driver_camera-device-common_MIMX8QX6)

include(driver_camera-receiver-common_MIMX8QX6)

include(driver_camera-receiver-isi_MIMX8QX6)

include(driver_clock_MIMX8QX6)

include(driver_common_MIMX8QX6)

include(device_MIMX8QX6_CMSIS_MIMX8QX6)

include(component_lpuart_adapter_MIMX8QX6)

include(component_lists_MIMX8QX6)

include(driver_lpuart_MIMX8QX6)

include(device_MIMX8QX6_startup_MIMX8QX6)

include(driver_scfw_api_MIMX8QX6)

include(CMSIS_Include_core_cm4_MIMX8QX6)

include(CMSIS_Include_common_MIMX8QX6)

include(CMSIS_Include_dsp_MIMX8QX6)

include(utilities_misc_utilities_MIMX8QX6)

include(device_MIMX8QX6_system_MIMX8QX6)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/isi_mipi_csi2_rgb565.bin)

