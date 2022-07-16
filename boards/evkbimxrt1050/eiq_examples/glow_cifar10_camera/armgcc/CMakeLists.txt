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


project(glow_cifar10_camera)

set(MCUX_SDK_PROJECT_NAME glow_cifar10_camera.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../source/image.c"
"${ProjDirPath}/../source/image.h"
"${ProjDirPath}/../gui_print/chgui.c"
"${ProjDirPath}/../gui_print/chgui.h"
"${ProjDirPath}/../gui_print/font.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../camera_support.c"
"${ProjDirPath}/../camera_support.h"
"${ProjDirPath}/../source/cifar10.h"
"${ProjDirPath}/../source/cifar10.o"
"${ProjDirPath}/../source/cifar10.weights.bin"
"${ProjDirPath}/../source/cifar10.weights.txt"
"${ProjDirPath}/../source/glow_cifar10_camera.c"
"${ProjDirPath}/../source/timer.c"
"${ProjDirPath}/../source/timer.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../peripherals.h"
"${ProjDirPath}/../peripherals.c"
"${ProjDirPath}/../scripts/glow_process_image.py"
"${ProjDirPath}/../scripts/glow_process_sound.py"
"${ProjDirPath}/../scripts/glow_show_detections.py"
"${ProjDirPath}/../scripts/glow_show_segmentation.py"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../gui_print
    ${ProjDirPath}/../source
    ${ProjDirPath}/../board
    ${ProjDirPath}/../../../../../middleware/eiq/glow/examples/cifar10_camera/evkbimxrt1050
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities
    ${ProjDirPath}/../../../../../devices/MIMXRT1052
    ${ProjDirPath}/../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../components/video/camera
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../components/video/camera/receiver
    ${ProjDirPath}/../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../components/video/camera/device/ov7725
    ${ProjDirPath}/../../../../../components/video/camera/device/mt9m114
    ${ProjDirPath}/../../../../../components/video/i2c
    ${ProjDirPath}/../../../../../components/video/camera/receiver/csi
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers
    ${ProjDirPath}/../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/DSP
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
)

# include modules
include(utility_debug_console_MIMXRT1052)

include(utility_assert_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)

include(driver_camera-device-common_MIMXRT1052)

include(driver_camera-common_MIMXRT1052)

include(driver_video-common_MIMXRT1052)

include(driver_camera-receiver-common_MIMXRT1052)

include(driver_camera-device-sccb_MIMXRT1052)

include(driver_camera-device-ov7725_MIMXRT1052)

include(driver_camera-device-mt9m114_MIMXRT1052)

include(driver_video-i2c_MIMXRT1052)

include(driver_camera-receiver-csi_MIMXRT1052)

include(driver_csi_MIMXRT1052)

include(driver_elcdif_MIMXRT1052)

include(driver_pxp_MIMXRT1052)

include(driver_dc-fb-common_MIMXRT1052)

include(driver_dc-fb-elcdif_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(driver_lpi2c_MIMXRT1052)

include(middleware_eiq_glow_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(component_serial_manager_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(component_serial_manager_uart_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_evkbimxrt1050_MIMXRT1052)

include(CMSIS_DSP_Source_MIMXRT1052)

include(CMSIS_Include_core_cm_MIMXRT1052)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


