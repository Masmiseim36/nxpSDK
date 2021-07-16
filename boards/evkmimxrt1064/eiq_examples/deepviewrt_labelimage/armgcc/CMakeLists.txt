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


project(deepviewrt_labelimage)

set(MCUX_SDK_PROJECT_NAME deepviewrt_labelimage.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../startup/startup_MIMXRT1064_armgcc.S"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../model.S"
"${ProjDirPath}/../source/data/sample.jpg"
"${ProjDirPath}/../source/models/mobilenet_v1_0.25_160.rtm"
"${ProjDirPath}/../source/models/mobilenet_v1_0.25_224_quant.rtm"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../cr_section_macros.h"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../source
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../components/video/camera/receiver
    ${ProjDirPath}/../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../components/video/camera/device/ov7725
    ${ProjDirPath}/../../../../../components/video/camera/device/mt9m114
    ${ProjDirPath}/../../../../../components/video/camera
    ${ProjDirPath}/../../../../../components/video/camera/receiver/csi
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../components/video/i2c
    ${ProjDirPath}/../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities
)

# include modules
include(utility_debug_console_lite_MIMXRT1064)

include(utility_assert_lite_MIMXRT1064)

include(driver_cache_armv7_m7_MIMXRT1064)

include(driver_ft5406_rt_MIMXRT1064)

include(driver_elcdif_MIMXRT1064)

include(driver_lpi2c_MIMXRT1064)

include(driver_camera-device-common_MIMXRT1064)

include(driver_camera-receiver-common_MIMXRT1064)

include(driver_camera-device-sccb_MIMXRT1064)

include(driver_camera-device-ov7725_MIMXRT1064)

include(driver_camera-device-mt9m114_MIMXRT1064)

include(driver_camera-common_MIMXRT1064)

include(driver_camera-receiver-csi_MIMXRT1064)

include(driver_csi_MIMXRT1064)

include(driver_video-common_MIMXRT1064)

include(driver_video-i2c_MIMXRT1064)

include(driver_flexram_MIMXRT1064)

include(driver_soc_flexram_allocate_MIMXRT1064)

include(driver_dc-fb-common_MIMXRT1064)

include(driver_dc-fb-elcdif_MIMXRT1064)

include(driver_pxp_MIMXRT1064)

include(middleware_eiq_deepviewrt_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_evkmimxrt1064_MIMXRT1064)

include(CMSIS_Include_core_cm_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/deepviewrt/lib/libdeepview-rt-cortex-m7f.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/deepviewrt/lib/libdeepview-rt-cortex-m7f.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


