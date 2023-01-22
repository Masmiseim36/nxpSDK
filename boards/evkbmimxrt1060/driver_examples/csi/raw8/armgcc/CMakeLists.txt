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


project(csi_raw8)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release sdram_debug sdram_release)

set(MCUX_SDK_PROJECT_NAME csi_raw8.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../csi_raw8.c"
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
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/video/camera/device/sccb
    ${SdkRootDirPath}/components/video/camera/device/ov7725
    ${SdkRootDirPath}/components/video/camera/device/mt9m114
    ${SdkRootDirPath}/components/video/i2c
    ${SdkRootDirPath}/components/video/camera/receiver/csi
    ${SdkRootDirPath}/components/video/display/dc/elcdif
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities/debug_console_lite
    ${SdkRootDirPath}/devices/MIMXRT1062/drivers
    ${SdkRootDirPath}/components/video/camera
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/components/video/camera/device
    ${SdkRootDirPath}/components/video/camera/receiver
    ${SdkRootDirPath}/components/video/display/dc
    ${SdkRootDirPath}/devices/MIMXRT1062
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1062/xip
    ${SdkRootDirPath}/boards/evkbmimxrt1060/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_camera-device-sccb_MIMXRT1062)

include(driver_camera-device-ov7725_MIMXRT1062)

include(driver_camera-device-mt9m114_MIMXRT1062)

include(driver_video-i2c_MIMXRT1062)

include(driver_camera-receiver-csi_MIMXRT1062)

include(driver_dc-fb-elcdif_MIMXRT1062)

include(utility_debug_console_lite_MIMXRT1062)

include(utility_assert_lite_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_csi_MIMXRT1062)

include(driver_elcdif_MIMXRT1062)

include(driver_camera-common_MIMXRT1062)

include(driver_video-common_MIMXRT1062)

include(driver_camera-device-common_MIMXRT1062)

include(driver_camera-receiver-common_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_dc-fb-common_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkbmimxrt1060_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


