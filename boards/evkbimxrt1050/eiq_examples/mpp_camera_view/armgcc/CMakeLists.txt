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


project(mpp_camera_view)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME mpp_camera_view.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/middleware/eiq/mpp/examples/camera_view/camera_view.c"
"${SdkRootDirPath}/middleware/eiq/mpp/examples/camera_view/mpp_config.h"
"${SdkRootDirPath}/middleware/eiq/mpp/boards/evkbimxrt1050/inc/board_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${SdkRootDirPath}/middleware/eiq/mpp/README.md"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../camera_support.c"
"${ProjDirPath}/../camera_support.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/middleware/eiq/mpp/examples/camera_view
    ${SdkRootDirPath}/middleware/eiq/mpp/boards/evkbimxrt1050/inc
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/video/camera/device
    ${SdkRootDirPath}/components/video/camera/receiver
    ${SdkRootDirPath}/components/video/camera/device/sccb
    ${SdkRootDirPath}/components/video/camera/device/ov7725
    ${SdkRootDirPath}/components/video/camera/device/mt9m114
    ${SdkRootDirPath}/components/video/camera
    ${SdkRootDirPath}/components/video/camera/receiver/csi
    ${SdkRootDirPath}/devices/MIMXRT1052/drivers
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/components/video/i2c
    ${SdkRootDirPath}/components/video/display/dc
    ${SdkRootDirPath}/components/video/display/dc/elcdif
    ${SdkRootDirPath}/devices/MIMXRT1052/utilities
    ${SdkRootDirPath}/devices/MIMXRT1052/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware/eiq
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT1052
    ${SdkRootDirPath}/devices/MIMXRT1052/xip
    ${SdkRootDirPath}/boards/evkbimxrt1050/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite
)

# include modules
include(driver_camera-device-common_MIMXRT1052)

include(driver_camera-receiver-common_MIMXRT1052)

include(driver_camera-device-sccb_MIMXRT1052)

include(driver_camera-device-ov7725_MIMXRT1052)

include(driver_camera-device-mt9m114_MIMXRT1052)

include(driver_camera-common_MIMXRT1052)

include(driver_camera-receiver-csi_MIMXRT1052)

include(driver_csi_MIMXRT1052)

include(driver_video-common_MIMXRT1052)

include(driver_video-i2c_MIMXRT1052)

include(driver_elcdif_MIMXRT1052)

include(driver_pxp_MIMXRT1052)

include(driver_dc-fb-common_MIMXRT1052)

include(driver_dc-fb-elcdif_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(utility_debug_console_lite_MIMXRT1052)

include(utility_assert_lite_MIMXRT1052)

include(middleware_eiq_mpp_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(driver_lpi2c_MIMXRT1052)

include(driver_cache_armv7_m7_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_evkbimxrt1050_MIMXRT1052)

include(CMSIS_Include_core_cm_MIMXRT1052)

include(middleware_freertos-kernel_MIMXRT1052)

include(middleware_freertos-kernel_extension_MIMXRT1052)

include(middleware_freertos-kernel_heap_4_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1052)

include(middleware_eiq_deepviewrt_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/mpp/lib/libmpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/deepviewrt/lib/libdeepview-rt-cortex-m7f.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/mpp/lib/libmpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/deepviewrt/lib/libdeepview-rt-cortex-m7f.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


