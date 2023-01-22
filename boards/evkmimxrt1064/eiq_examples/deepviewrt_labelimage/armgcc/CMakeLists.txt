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

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME deepviewrt_labelimage.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
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

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../source
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1064/utilities
    ${SdkRootDirPath}/devices/MIMXRT1064/utilities/debug_console_lite
    ${SdkRootDirPath}/devices/MIMXRT1064/drivers
    ${SdkRootDirPath}/components/ft5406_rt
    ${SdkRootDirPath}/components/video/camera/device
    ${SdkRootDirPath}/components/video/camera/receiver
    ${SdkRootDirPath}/components/video/camera/device/sccb
    ${SdkRootDirPath}/components/video/camera/device/ov7725
    ${SdkRootDirPath}/components/video/camera/device/mt9m114
    ${SdkRootDirPath}/components/video/camera
    ${SdkRootDirPath}/components/video/camera/receiver/csi
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/components/video/i2c
    ${SdkRootDirPath}/components/video/display/dc
    ${SdkRootDirPath}/components/video/display/dc/elcdif
    ${SdkRootDirPath}/middleware/eiq
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT1064
    ${SdkRootDirPath}/devices/MIMXRT1064/xip
    ${SdkRootDirPath}/boards/evkmimxrt1064/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
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

include(driver_dc-fb-common_MIMXRT1064)

include(driver_dc-fb-elcdif_MIMXRT1064)

include(driver_pxp_MIMXRT1064)

include(middleware_eiq_deepviewrt_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_evkmimxrt1064_MIMXRT1064)

include(CMSIS_Include_core_cm_MIMXRT1064)

include(driver_soc_flexram_allocate_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/deepviewrt/lib/libdeepview-rt-cortex-m7f.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/deepviewrt/lib/libdeepview-rt-cortex-m7f.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


