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


project(glow_cifar10_camera_cm7)

set(MCUX_SDK_PROJECT_NAME glow_cifar10_camera_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../source/labels.h"
"${ProjDirPath}/../source/parameter.h"
"${ProjDirPath}/../source/weights.h"
"${ProjDirPath}/../board/board_init.h"
"${ProjDirPath}/../board/board_init.c"
"${ProjDirPath}/../source/image/demo_config.h"
"${ProjDirPath}/../source/image/image_data.h"
"${ProjDirPath}/../source/image/image.h"
"${ProjDirPath}/../source/image/image_capture.c"
"${ProjDirPath}/../source/image/image_conv_rotate.c"
"${ProjDirPath}/../source/image/image_decode_raw.c"
"${ProjDirPath}/../source/image/image_resize_nearest.c"
"${ProjDirPath}/../source/image/image_draw.c"
"${ProjDirPath}/../source/image/image_utils.h"
"${ProjDirPath}/../source/cifar10.h"
"${ProjDirPath}/../source/cifar10.o"
"${ProjDirPath}/../source/cifar10.weights.bin"
"${ProjDirPath}/../source/cifar10.weights.txt"
"${ProjDirPath}/../source/glow_cifar10_camera.c"
"${ProjDirPath}/../source/image.c"
"${ProjDirPath}/../source/image.h"
"${ProjDirPath}/../source/timer.c"
"${ProjDirPath}/../source/timer.h"
"${ProjDirPath}/../eiq/cmsis_nn/Include/arm_nn_tables.h"
"${ProjDirPath}/../eiq/cmsis_nn/Include/arm_nnfunctions.h"
"${ProjDirPath}/../eiq/cmsis_nn/Include/arm_nnsupportfunctions.h"
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
    ${ProjDirPath}/../source/image
    ${ProjDirPath}/../source/video
    ${ProjDirPath}/../eiq/cmsis_nn/Include
    ${ProjDirPath}/../source
    ${ProjDirPath}/../board
    ${ProjDirPath}/../../../../../middleware/eiq/glow/examples/cifar10_camera/evkmimxrt1170
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities
    ${ProjDirPath}/../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../components/video/display/mipi_dsi_cmd
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../components/video/display/rm68191
    ${ProjDirPath}/../../../../../components/video/display/rm68200
    ${ProjDirPath}/../../../../../components/video/display
    ${ProjDirPath}/../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../components/video/camera
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../components/video/camera/receiver
    ${ProjDirPath}/../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../components/video/camera/device/ov5640
    ${ProjDirPath}/../../../../../components/video/i2c
    ${ProjDirPath}/../../../../../components/video/camera/receiver/csi
    ${ProjDirPath}/../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../components/video/display/dc/lcdifv2
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../CMSIS/DSP
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
)

# include modules
include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)

include(driver_display-mipi-dsi-cmd_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(driver_mipi_dsi_split_MIMXRT1176_cm7)

include(driver_display-rm68191_MIMXRT1176_cm7)

include(driver_display-rm68200_MIMXRT1176_cm7)

include(driver_display-common_MIMXRT1176_cm7)

include(driver_camera-device-common_MIMXRT1176_cm7)

include(driver_camera-common_MIMXRT1176_cm7)

include(driver_video-common_MIMXRT1176_cm7)

include(driver_camera-receiver-common_MIMXRT1176_cm7)

include(driver_camera-device-sccb_MIMXRT1176_cm7)

include(driver_camera-device-ov5640_MIMXRT1176_cm7)

include(driver_mipi_csi2rx_MIMXRT1176_cm7)

include(driver_video-i2c_MIMXRT1176_cm7)

include(driver_camera-receiver-csi_MIMXRT1176_cm7)

include(driver_csi_MIMXRT1176_cm7)

include(driver_elcdif_MIMXRT1176_cm7)

include(driver_pxp_MIMXRT1176_cm7)

include(driver_dc-fb-common_MIMXRT1176_cm7)

include(driver_dc-fb-elcdif_MIMXRT1176_cm7)

include(driver_dc-fb-lcdifv2_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_soc_src_MIMXRT1176_cm7)

include(driver_soc_mipi_csi2rx_MIMXRT1176_cm7)

include(middleware_eiq_worker_MIMXRT1176_cm7)

include(middleware_eiq_worker_video_MIMXRT1176_cm7)

include(middleware_eiq_gui_printf_MIMXRT1176_cm7)

include(CMSIS_DSP_Library_MIMXRT1176_cm7)

include(middleware_eiq_glow_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(driver_lcdifv2_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


