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


project(glow_lenet_mnist_camera)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME glow_lenet_mnist_camera.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

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
"${ProjDirPath}/../source/lenet_mnist.h"
"${ProjDirPath}/../source/lenet_mnist.o"
"${ProjDirPath}/../source/lenet_mnist.weights.bin"
"${ProjDirPath}/../source/lenet_mnist.weights.txt"
"${ProjDirPath}/../source/glow_lenet_mnist_camera.c"
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

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../gui_print
    ${ProjDirPath}/../source
    ${ProjDirPath}/../board
    ${SdkRootDirPath}/middleware/eiq/glow/examples/lenet_mnist_camera/evkbimxrt1050
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1052/utilities
    ${SdkRootDirPath}/devices/MIMXRT1052
    ${SdkRootDirPath}/components/video/camera/device
    ${SdkRootDirPath}/components/video/camera
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/components/video/camera/receiver
    ${SdkRootDirPath}/components/video/camera/device/sccb
    ${SdkRootDirPath}/components/video/camera/device/ov7725
    ${SdkRootDirPath}/components/video/camera/device/mt9m114
    ${SdkRootDirPath}/components/video/i2c
    ${SdkRootDirPath}/components/video/camera/receiver/csi
    ${SdkRootDirPath}/devices/MIMXRT1052/drivers
    ${SdkRootDirPath}/components/video/display/dc
    ${SdkRootDirPath}/components/video/display/dc/elcdif
    ${SdkRootDirPath}/middleware/eiq
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/devices/MIMXRT1052/xip
    ${SdkRootDirPath}/boards/evkbimxrt1050/xip
    ${SdkRootDirPath}/CMSIS/DSP
    ${SdkRootDirPath}/CMSIS/Core/Include
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


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


